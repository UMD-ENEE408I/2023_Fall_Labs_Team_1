#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>

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

void setup() {

  // Stop the right motor by setting pin 14 low
  // this pin floats high or is pulled
  // high during the bootloader phase for some reason
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  delay(2000);

  Serial.begin(9600);

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
  
  RemoteClient = Server.available();
  if (RemoteClient){
    while(RemoteClient.connected()){
      while(RemoteClient.available()>0){
        char c = RemoteClient.read();
        RemoteClient.write(c);
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