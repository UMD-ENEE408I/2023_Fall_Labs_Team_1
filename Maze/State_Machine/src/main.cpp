#include <Arduino.h>
#include <Adafruit_MCP3008.h>

#include <Encoder.h>

int robot = 1;  // SET THIS.

int state = 1;  // Stage of the course, 1, 2, 3, or 4. Global so it isn't reset when loop() restarts.

Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;

const unsigned int ADC_1_CS = 2;
const unsigned int ADC_2_CS = 17;

int adc1_buf[8];
int adc2_buf[8];

int ir_led[13];

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

const unsigned int base_pwm = 350; // Do not give max PWM. Robot will move fast. Robot 1: 400, robot 2: 300.

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

void readADC(int color[13]) {
  int avg = 0;
  double center = 0;
  double center_sum = 0;
  double center_count = 0;
  double count = 0;
  
  //white < black.
  int black_thres = 600;  // Initialize line sensor white threshold. Following if statement sets for a specific robot.

  if (robot == 1){ 
    black_thres = 600;
  }
  else {
    if (robot == 2){ 
      black_thres = 675;
    }
  }

  for (int i = 0; i < 8; i++) {
    adc1_buf[i] = adc1.readADC(i);  // Odd sensors. Starting at lower sensor number.
    adc2_buf[i] = adc2.readADC(i);  // Even sensors. Starting at lower sensor number.
    

    if (i<7) {  // Handle odd sensors.
      if (adc1_buf[i]>black_thres) color[2*i] = 0; // Black.
      //else if (adc1_buf[i]>white_thres) color[2*i] = 1; // White.
      // Else yellow as initialized.
      //color[2*i] = adc1_buf[i];
    }
    

    if (i<6) {  // Handle even sensors.
      if (adc2_buf[i]>black_thres) color[2*i + 1] = 0; // Black.
      //else if (adc1_buf[i]>white_thres) color[2*i + 1] = 1; // White.
      // Else yellow as initialized.
      //color[2*i + 1] = adc2_buf[i];
    }
    

  }
    // avg = avg / 13;
    // //Serial.print("Average: ");
    // //Serial.print(avg);
    // //Serial.print("\t");
    // if (avg > 600) {
    //   Serial.print("Surface: 0\t");
    // }
    // else{
    //   Serial.print("Surface: 1\t");
    // }
    // center = (center_sum/center_count)-1;
    // Serial.print("Center: "); Serial.print(center); Serial.print("\t");
    
}

int count_white(int color[13]) {
  int i = 0;
  int num_white = 0;
  for (i == 0; i < 13; i++) num_white = num_white + color[i];
  return num_white;
}

void increment_state(int color[13]) {
  int num_white = 0;
  readADC(color);
  num_white = count_white(color);
  while (num_white > 9){ 
  readADC(color);
  num_white = count_white(color);
  // LINE FOLLOW.
  }
  state++;
}

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

void follow_line(){

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

void loop(){

  int color[13] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; 
  // color[i] represents the color for sensor i + 1. White = 1; black = 0;

  int t_start = micros();
  readADC(color);
  int t_end = micros();

  int num_white = 0;

  num_white = count_white(color);

  // // Print sensor values.
  // for (i = 0; i < 13; i++){
  //   Serial.print(color[i]); Serial.print('\t');
  // }

  // Serial.println();

  // if (num_white <= 3) { // Line follow.
  //   Serial.print("Line follow."); Serial.println();
  // }
  // else{ // Other logic.
  //   Serial.print("Other Logic."); Serial.println();

  switch(state) {
    case 1:
    
      if (num_white > 9){
        while (num_white > 9){ 
        readADC(color);
        num_white = count_white(color);
        follow_line();
        }
        state++;
      } 
      else{
        if (num_white > 3){ // Reached right angle in line.
          // Turn 90 deg towards the direction with more white. Not yet implemented.
        }
        follow_line();
      } 
      
      break;

    case 2:

    
    
      break;
    case 3:
      break;
    case 4:
      break;
    default:
      // Course finished; do nothing.
      break;
  }

      Serial.print(state); Serial.println();



  // delay(100);

}
