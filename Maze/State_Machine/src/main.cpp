#include <Arduino.h>
#include <Adafruit_MCP3008.h>

Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;

const unsigned int ADC_1_CS = 2;
const unsigned int ADC_2_CS = 17;

int adc1_buf[8];
int adc2_buf[8];

int robot = 1;  // SET THIS.

int state = 1;  // Stage of the course, 1, 2, 3, or 4. Global so it isn't reset when loop() restarts.

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

}

void loop() {

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
        // LINE FOLLOW.
        }
        state++;
      } 
      else{
        if (num_white > 3){
          // Turn 90 deg based towards the direction with more white.
        }
        else {Serial.print("Line follow."); Serial.println();}  //LINE FOLLOW.
      } 
      
      break;

    case 2:

    
    
      break;
    case 3:
      break;
    case 4:
      break;
  }

      Serial.print(state); Serial.println();



  // delay(100);

}
