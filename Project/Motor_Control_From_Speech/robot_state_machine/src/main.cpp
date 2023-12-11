/* 
Authors: Daniel McLawhorn and Kent Chesley
Date: 11/2023
(Original code by Daniel from Project/Motor_Control_From_Speech/WiFi_Audio was adapted by Kent to implement robot's 
finite state machine.)

Code for the robot which receives from the Nvidia the robot state, left PWM, and right PWM values, and sets the
motors accordingly. 

robot states are as follows:
0: Stall.
-1: Turn left.
1: Turn right.
-2: Drive backwards.
2: Drive forwards.

*/

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <Adafruit_MCP3008.h>
#include <Adafruit_MPU6050.h>
#include <Encoder.h>
#include <string>
#include <sstream>

const unsigned int BUZZ = 26; // Check schematic and see pin connection to buzzer
const unsigned int BUZZ_CHANNEL = 2; //Selecting PWM channel 0
const unsigned int octave = 5;

const unsigned int M1_ENC_A = 39;
const unsigned int M1_ENC_B = 38;
const unsigned int M2_ENC_A = 37;
const unsigned int M2_ENC_B = 36;

const unsigned int M1_IN_1=13; //GPIO13 on Heltec board
const unsigned int M1_IN_2=12; //GPIO12 on Heltec board
const unsigned int M2_IN_1=25; //GPIO25 on Heltec board
const unsigned int M2_IN_2=14; //GPIO14 on Heltec board

const unsigned int M1_IN_1_CHANNEL=8;
const unsigned int M1_IN_2_CHANNEL=9;
const unsigned int M2_IN_1_CHANNEL=10;
const unsigned int M2_IN_2_CHANNEL=11;

const unsigned int M1_I_SENSE = 35;
const unsigned int M2_I_SENSE = 34;

const unsigned int base_pwm = 300; // Do not give max PWM. Robot will move fast

const int freq = 5000;
const int resolution = 10;

void M1_forward(int pwm_val){

  // Left motor Forward

  ledcWrite(M1_IN_1_CHANNEL, 0);
  ledcWrite(M1_IN_2_CHANNEL, pwm_val);
}

void M2_forward(int pwm_val){

  // Right motor Forward

  ledcWrite(M2_IN_1_CHANNEL, 0);
  ledcWrite(M2_IN_2_CHANNEL, pwm_val);
}

void M1_backward(int pwm_val){

  // Left motor backward

  ledcWrite(M1_IN_1_CHANNEL, pwm_val);
  ledcWrite(M1_IN_2_CHANNEL, 0);
}

void M2_backward(int pwm_val){

  // Right motor backward

  ledcWrite(M2_IN_1_CHANNEL, pwm_val);
  ledcWrite(M2_IN_2_CHANNEL, 0);
}

void M1_stop(){
  ledcWrite(M1_IN_1_CHANNEL, 1023);
  ledcWrite(M1_IN_2_CHANNEL, 1023);
}

void M2_stop(){
  ledcWrite(M2_IN_1_CHANNEL, 1023);
  ledcWrite(M2_IN_2_CHANNEL, 1023);
}




const char* ssid = "D's iPhone";
const char* password = "cobg7qzqn8yrv";

const uint ServerPort = 80;

WiFiServer Server(ServerPort);

WiFiClient RemoteClient;

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

void strTo3Int(std::string input, int output[3]) {

  // Reference: 
  // https://stackoverflow.com/questions/1321137/convert-string-containing-several-numbers-into-integers.

  std::stringstream stream(input);

  int i;

  for(i = 0; i < 3; i++){
    stream >> output[i];
  }

}

void setup() {

  // Stop the right motor by setting pin 14 low
  // this pin floats high or is pulled
  // high during the bootloader phase for some reason
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  delay(2000);

  Serial.begin(9600);

  ledcSetup(M1_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M1_IN_2_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_2_CHANNEL, freq, resolution);

  ledcAttachPin(M1_IN_1, M1_IN_1_CHANNEL);
  ledcAttachPin(M1_IN_2, M1_IN_2_CHANNEL);
  ledcAttachPin(M2_IN_1, M2_IN_1_CHANNEL);
  ledcAttachPin(M2_IN_2, M2_IN_2_CHANNEL);

  ledcAttachPin(BUZZ, BUZZ_CHANNEL);

  pinMode(M1_I_SENSE, INPUT);
  pinMode(M2_I_SENSE, INPUT);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  
  initWiFi();
  delay(100);

  Serial.println("Setup done");

  AdvertiseServices("Heltec");

  Server.begin();

  
}

void loop() {
  //CheckForConnections();

  //SendValue();
  char c;
  int commands[3];

  RemoteClient = Server.available();
  if (RemoteClient){
    while(RemoteClient.connected()){

      std::string command = "";

      while(RemoteClient.available()>0){
        c = RemoteClient.read();
        command += c;
      }

      // Serial.println(command.c_str());  // Reference: https://techoverflow.net/2022/09/07/how-to-serial-println-a-stdstring/.
      //strTo4Int(command, commands);
      strTo3Int(command, commands);
      int rState = commands[0];
      int lPWM = commands[1];
      int rPWM = commands[2];
      if(rPWM>=1000){
        rPWM = rPWM/10;
      }
      //int time = commands[3];
      // Serial.print("\n");
      Serial.print(rState); Serial.print(", "); Serial.print(lPWM); Serial.print(", "); Serial.println(rPWM);
      // Serial.print("\n");


      switch (rState)
      {
      case -1:  // Turn left.
        M1_backward(lPWM);
        M2_forward(rPWM);/*
        delay(time);
        M1_stop();
        M2_stop();
        delay(1000);*/
        //ledcWriteTone(BUZZ_CHANNEL, 0);
        break;

      case 1: // Turn right.
        M1_forward(lPWM);
        M2_backward(rPWM);/*
        delay(time);
        M1_stop();
        M2_stop();
        delay(3000);*/
        //ledcWriteTone(BUZZ_CHANNEL, 0);
        break;

      case -2:  // Drive backwards.
        M1_backward(lPWM);
        M2_backward(rPWM);/*
        delay(time);
        M1_stop();
        M2_stop();
        delay(3000);*/
        //ledcWriteTone(BUZZ_CHANNEL, 0);
        break;

      case 2: // Drive forwards.
        M1_forward(lPWM);
        M2_forward(rPWM);/*
        delay(time);
        M1_stop();
        M2_stop();
        delay(3000);*/
        //ledcWriteNote(BUZZ_CHANNEL, NOTE_C, octave);
        break;
      
      default:  // Stall. State 0 used.
        M1_stop();
        M2_stop();
        break;
      }

      delay(10);
    }
    RemoteClient.stop();
    Serial.println("Client disconnected");
  }
  
  

  /* Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000); */
  

}