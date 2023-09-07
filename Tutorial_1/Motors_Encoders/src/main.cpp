#include <Arduino.h>
#include <Encoder.h>

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

const unsigned int PWM_VALUE = 100; // Do not give max PWM. Robot will move fast

const int freq = 5000;
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
  // Stop the right motor by setting pin 14 low
  // this pin floats high or is pulled
  // high during the bootloader phase for some reason
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  delay(100);

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
  // Create the encoder objects after the motor has
  // stopped, else some sort exception is triggered
  Encoder enc1(M1_ENC_A, M1_ENC_B);
  Encoder enc2(M2_ENC_A, M2_ENC_B);

  while(true) {
    long enc1_value = enc1.read();
    long enc2_value = enc2.read();
    while(enc1.read() <= enc1_value+360 && enc2.read() >= enc2_value-360){
      delay(5);
      Serial.print(enc1_value); Serial.print("\t");
      Serial.print(enc2_value); Serial.print("\n");
      delay(5);
      M1_forward();
      M2_forward();
    }
    M1_stop();
    M2_stop();
    delay(5000);
    while(enc1.read() >= enc1_value && enc2.read() <= enc2_value){
      delay(5);
      Serial.print(enc1_value); Serial.print("\t");
      Serial.print(enc2_value); Serial.print("\n");
      delay(5);
      M1_backward();
      M2_backward();
    }
    M1_stop();
    M2_stop();
    delay(5000);

  }
}
