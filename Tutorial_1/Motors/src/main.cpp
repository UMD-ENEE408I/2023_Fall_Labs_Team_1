#include <Arduino.h>

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

const unsigned int PWM_VALUE = 255; // Do not give max PWM. Robot will move fast

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

void M1_forward(){
  ledcWrite(M1_IN_1_CHANNEL, 0);
  ledcWrite(M1_IN_2_CHANNEL, PWM_VALUE);
}

void M2_forward(){
  ledcWrite(M2_IN_1_CHANNEL, 0);
  ledcWrite(M2_IN_2_CHANNEL, PWM_VALUE);
}

void M1_backward(){
  ledcWrite(M1_IN_1_CHANNEL, PWM_VALUE);
  ledcWrite(M1_IN_2_CHANNEL, 0);
}

void M2_backward(){
  ledcWrite(M2_IN_1_CHANNEL, PWM_VALUE);
  ledcWrite(M2_IN_2_CHANNEL, 0);
}

void M1_stop(){
  ledcWrite(M1_IN_1_CHANNEL, PWM_VALUE);
  ledcWrite(M1_IN_2_CHANNEL, PWM_VALUE);
}

void M2_stop(){
  ledcWrite(M2_IN_1_CHANNEL, PWM_VALUE);
  ledcWrite(M2_IN_2_CHANNEL, PWM_VALUE);
}

void setup() {
  Serial.begin(115200);
  
  ledcSetup(M1_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M1_IN_2_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_2_CHANNEL, freq, resolution);

  ledcAttachPin(M1_IN_1, M1_IN_1_CHANNEL);
  ledcAttachPin(M1_IN_2, M1_IN_2_CHANNEL);
  ledcAttachPin(M2_IN_1, M2_IN_1_CHANNEL);
  ledcAttachPin(M2_IN_2, M2_IN_2_CHANNEL);

  pinMode(M1_I_SENSE, INPUT);
  pinMode(M2_I_SENSE, INPUT);
}

void loop() {
    M1_stop();
    M2_stop();
    delay(1000);
    M1_forward();
    M2_forward();
    delay(1000);
    M1_stop();
    M2_stop();
    delay(500);
    M1_backward();
    M2_backward();
    delay(1000);
}
