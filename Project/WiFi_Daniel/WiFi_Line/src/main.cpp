#include <Arduino.h>
#include <Adafruit_MCP3008.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <bits/stdc++.h>
#include <string>


const char* ssid = "D's iPhone";
const char* password = "cobg7qzqn8yrv";

const uint ServerPort = 80;

WiFiServer Server(ServerPort);

WiFiClient RemoteClient;

Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;

const unsigned int ADC_1_CS = 2;
const unsigned int ADC_2_CS = 17;

int adc1_buf[8];
int adc2_buf[8];

int ir_led[13];

float prev_line_pos = 6;
int mid_line = 6;

float position(){
  int avg = 0;
  float center = 0;
  int white_count = 0;
  //black = 727
  //white = 622
  //yellow = 580
  for (int i = 0; i < 8; i++) {
    adc1_buf[i] = adc1.readADC(i);
    adc2_buf[i] = adc2.readADC(i);
    

    if (i<7) {
      //Serial.print(adc1_buf[i]); Serial.print("\t");    
      avg = avg + adc1_buf[i];
      if (adc1_buf[i]>675){
        ir_led[2*i] = 0;
        // Serial.print("0\t");
      }
      else{
        ir_led[2*i] = 1;
        white_count+=1;
        // Serial.print("1\t");
        // center_sum = center_sum + count;
        // center_count = center_count + 1;

      }
    }

    if (i<6) { //Even sensors
      //Serial.print(adc2_buf[i]); Serial.print("\t");
      avg = avg + adc2_buf[i];
      if (adc2_buf[i]>675){
        ir_led[2*i+1] = 0;
        // Serial.print("0\t");
      }
      else{
        ir_led[2*i+1] = 1;
        white_count+=1;

        // Serial.print("1\t");
        // center_sum = center_sum + count;
        // center_count = center_count + 1;
      }
    }
  }

  for (int i=0; i<13; i++) {
    // Serial.print(ir_led[i]);
    // Serial.print('\t');

    center += i* ir_led[i]; 
  }

  if (white_count != 0) {
    center = center/white_count;
  } else {
    center = prev_line_pos;
  }

  // Serial.print("Center: "); Serial.print(center); Serial.print("\t"); Serial.println();

  prev_line_pos = center;
  return center;
}

void CheckForConnections(){
  if (Server.hasClient()){
    if (RemoteClient.connected()){
      Serial.println("Connection rejected");
      Server.available().stop();
    }
    else{
      Serial.println("Connection accepted");
      RemoteClient = Server.available();
    }
  }
}

void SendValue(){
  if (RemoteClient.connected()){
    RemoteClient.println("Hello");
  }
}

void AdvertiseServices(const char *MyName)
{
  if (MDNS.begin(MyName))
  {
    Serial.println(F("mDNS responder started"));
    Serial.print(F("I am: "));
    Serial.println(MyName);
 
    // Add service to MDNS-SD
    MDNS.addService("n8i-mlp", "tcp", 23);
  }
  else
  {
    while (1) 
    {
      Serial.println(F("Error setting up MDNS responder"));
      delay(1000);
    }
  }
}

void initWiFi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  // Stop the right motor by setting pin 14 low
  // this pin floats high or is pulled
  // high during the bootloader phase for some reason
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  delay(100);

  Serial.begin(9600);
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  
  initWiFi();
  delay(100);
  Serial.println("Setup done");

  AdvertiseServices("Heltec");

  Server.begin();

  adc1.begin(ADC_1_CS);  
  adc2.begin(ADC_2_CS);

}

void loop() {
  float pos = position();
  std::string pos_s = std::to_string(pos);
  std::string spaces = "  \n";
  std::string pos_sp = pos_s + spaces;
  RemoteClient = Server.available();
  if (RemoteClient){
    while(RemoteClient.connected()){
      for (int i=0; i<strlen(pos_sp.c_str()); i++){
          RemoteClient.write(pos_sp[i]);
      }
      delay(10);
      pos = position();
      pos_s = std::to_string(pos);
      pos_sp = pos_s + spaces;
      
    }
    RemoteClient.stop();
    Serial.println("Client disconnected");
  }
}
