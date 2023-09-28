#include <Arduino.h>
#include <Adafruit_MCP3008.h>

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

const unsigned int base_pwm = 300; // Do not give max PWM. Robot will move fast

const int freq = 5000;
const int resolution = 10;

float prev_line_pos = 6;
int mid_line = 6;


float error = 0;
float last_error = 0;
float total_error = 0;

int K_p_lf = 25;
int K_d_lf = 1500;
int K_i_lf = 0;

void M1_forward(int pwm_val){

  //Left motor Forward
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



Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;

const unsigned int ADC_1_CS = 2;
const unsigned int ADC_2_CS = 17;

int adc1_buf[8];
int adc2_buf[8];

int ir_led[13];

int pid_val(float error, float last_error, float total_error) {

  int pid_val = K_p_lf * error + K_d_lf * (error - last_error) + K_i_lf * (error+total_error);
  return pid_val;
}

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

void setup() {
  // Stop the right motor by setting pin 14 low
  // this pin floats high or is pulled
  // high during the bootloader phase for some reason
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  delay(100);

  Serial.begin(115200);

  adc1.begin(ADC_1_CS);  
  adc2.begin(ADC_2_CS);

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
  float pos = position();

  error = pos - mid_line;

  // Control motors

  int pid_v = pid_val(error, last_error, total_error);
  int left_motor_pwm = base_pwm-pid_v;
  int right_motor_pwm = base_pwm+pid_v;

  Serial.print(left_motor_pwm);
  Serial.print('\t');
  Serial.println(right_motor_pwm);

  M1_forward(left_motor_pwm);
  M2_forward(right_motor_pwm);
  
  last_error = error;
  total_error+=error;

  
  

}
