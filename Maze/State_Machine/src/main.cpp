#include <Arduino.h>
#include <Adafruit_MCP3008.h>
#include <Adafruit_MPU6050.h>

#include <Encoder.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <chrono>  // For using now() function.

// *******IMPORTANT********
int robot = 2;  // SET THIS.
// *******IMPORTANT********

int state = 1;  // Stage of the course, 1, 2, 3, or 4. Global so it isn't reset when loop() restarts.
Adafruit_MPU6050 mpu;

Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;

Adafruit_MPU6050 mpu;

const unsigned int ADC_1_CS = 2;
const unsigned int ADC_2_CS = 17;

int adc1_buf[8];
int adc2_buf[8];

int ir_led[13];

// M1 is left, M2 is right.

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

const float M_I_COUNTS_TO_A = (3.3 / 1024.0) / 0.120;

const unsigned int turn_pwm = 425; // Set fairly low to prevent overshoot.

int black_thres = 600;  // Initialize line sensor white threshold. Setup loop sets for a specific robot.
unsigned int base_pwm = 350; // Initialize base PWM value. Setup loop sets for a specific robot.

const int freq = 5000;
const int resolution = 10;

float prev_line_pos = 6;
int mid_line = 6;


float error = 0;
float last_error = 0;
float total_error = 0;

int K_p_lf = 25;
int K_d_lf = 1700;
int K_i_lf = 0;

// IMU variables initialization:
double imu_integral = 0;  // Used to integrate (Riemann sum) the angular velocity.
double sample = 0;
auto last_time = std::chrono::high_resolution_clock::now();
double elapsed_time_ms = 0;

void readADC(int color[13]) {
  int avg = 0;
  double center = 0;
  double center_sum = 0;
  double center_count = 0;
  double count = 0;

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
  // The returned value is the average position of the sensors detecting white where the lowest sensor is 
  // in poistion 0 and the highest is in position 12.
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
      if (adc1_buf[i]>black_thres){
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
      if (adc2_buf[i]>black_thres){
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

void turn(int dir) {
  // dir: -1 = 90 deg left, 1 = 90 deg right, 0 = 180 deg.
  
  sensors_event_t a, g, temp;

  switch (dir)
  {
  case -1:
    // Rotate left 90 deg.
    /* Get new sensor events with the readings */
    mpu.getEvent(&a, &g, &temp);

    M1_backward(turn_pwm);
    M2_forward(turn_pwm);
    last_time = std::chrono::high_resolution_clock::now();  // Time in ms.
    imu_integral = 0;
    do
    {
      // "Integrate" the angular velocity to get angular change.
      delay(10);
      mpu.getEvent(&a, &g, &temp);  // Update measurment.
      sample = g.gyro.z;
      elapsed_time_ms = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - last_time).count();
      // For reference, see Sumsuddin Shojib' answer here: 
      // https://stackoverflow.com/questions/728068/how-to-calculate-a-time-difference-in-c
      last_time = std::chrono::high_resolution_clock::now();
      imu_integral = imu_integral + sample*elapsed_time_ms/1000;
    } while (abs(imu_integral) < (3.1416/2 - 3.1416/10)); // Offset of pi/10 because the robot's rotational momentum
    // causes it to overshoot slightly which will make up the difference.
    
    // Bring both motors to full and complete stop so that they don't have any non-zero inertia that keeps the robot 
    // turning once line following is resumed.
    M1_stop();
    M2_stop();
    delay(50);  // Probably good to replace this with something that checks the IMU or wheel encoder.

    break;

  case 1:
    // Rotate right 90 deg.
    /* Get new sensor events with the readings */
    mpu.getEvent(&a, &g, &temp);

    M1_forward(turn_pwm);
    M2_backward(turn_pwm);
    last_time = std::chrono::high_resolution_clock::now();  // Time in ms.
    imu_integral = 0;
    do
    {
      // "Integrate" the angular velocity to get angular change.
      delay(10);
      mpu.getEvent(&a, &g, &temp);  // Update measurment.
      sample = g.gyro.z;
      elapsed_time_ms = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - last_time).count();
      // For reference, see Sumsuddin Shojib' answer here: 
      // https://stackoverflow.com/questions/728068/how-to-calculate-a-time-difference-in-c
      last_time = std::chrono::high_resolution_clock::now();
      imu_integral = imu_integral + sample*elapsed_time_ms/1000;
    } while (abs(imu_integral) < (3.1416/2 - 3.1416/10)); // Offset of pi/10 because the robot's rotational momentum
    // causes it to overshoot slightly which will make up the difference.

    // Bring both motors to full and complete stop so that they don't have any non-zero inertia that keeps the robot 
    // turning once line following is resumed.
    M1_stop();
    M2_stop();
    delay(50);  // Probably good to replace this with something that checks the IMU or wheel encoder.

    break;
  
  default:  // Case 0.
    // Rotate right 180 deg.
    /* Get new sensor events with the readings */
    mpu.getEvent(&a, &g, &temp);

    M1_forward(turn_pwm);
    M2_backward(turn_pwm);
    last_time = std::chrono::high_resolution_clock::now();  // Time in ms.
    imu_integral = 0;
    do
    {
      // "Integrate" the angular velocity to get angular change.
      delay(10);
      mpu.getEvent(&a, &g, &temp);  // Update measurment.
      sample = g.gyro.z;
      elapsed_time_ms = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - last_time).count();
      // For reference, see Sumsuddin Shojib' answer here: 
      // https://stackoverflow.com/questions/728068/how-to-calculate-a-time-difference-in-c
      last_time = std::chrono::high_resolution_clock::now();
      imu_integral = imu_integral + sample*elapsed_time_ms/1000;
    } while (abs(imu_integral) < (3.1416 - 3.1416/10)); // Offset of pi/10 because the robot's rotational momentum
    // causes it to overshoot slightly which will make up the difference.

    // Bring both motors to full and complete stop so that they don't have any non-zero inertia that keeps the robot 
    // turning once line following is resumed.
    M1_stop();
    M2_stop();
    delay(50);  // Probably good to replace this with something that checks the IMU or wheel encoder.

    break;
  }

}

void setup() {
  // Stop the right motor by setting pin 14 low
  // this pin floats high or is pulled
  // high during the bootloader phase for some reason
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  delay(100);

  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  pinMode(ADC_1_CS, OUTPUT);
  pinMode(ADC_2_CS, OUTPUT);

  digitalWrite(ADC_1_CS, HIGH); // Without this the ADC's write
  digitalWrite(ADC_2_CS, HIGH); // to the SPI bus while the nRF24 is!!!!

  adc1.begin(ADC_1_CS);  
  adc2.begin(ADC_2_CS);

  // IMU setup.

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);

  // Motor setup.

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

  //white < black.

  if (robot == 1){ 
    black_thres = 600;
    base_pwm = 350; // Do not give max PWM. Robot will move fast.
  }
  else {
    if (robot == 2){ 
      black_thres = 675;
      base_pwm = 300; // Do not give max PWM. Robot will move fast.
    }
  }

}

// Initializations.
double integral = 0;  // Used to integrate (Riemann sum) the angular velocity.
double sample = 0;
auto last_time = std::chrono::high_resolution_clock::now(); // For reference, see Sumsuddin Shojib' answer here: 
// https://stackoverflow.com/questions/728068/how-to-calculate-a-time-difference-in-c
double elapsed_time_ms = 0;

void loop(){

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  int color[13] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; 
  // color[i] represents the color for sensor i + 1. White = 1; black = 0;

  int t_start = micros();
  readADC(color);
  int t_end = micros();

  int num_white = 0;

  num_white = count_white(color);

  int pos = 0;  // Holds position of center of line.

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
    
      if (num_white > 9){ // Reached end of segment.
        while (num_white > 9){ 
        readADC(color);
        num_white = count_white(color);
        follow_line();
        }
        state++;
      } 
      else{
        if (num_white > 5){ // Reached right angle in line.

          // Drive foward briefly so that the robot is centered over the line after the turn.
          M1_forward(base_pwm);
          M2_forward(base_pwm);
          delay(150); // Needs to be updated to use the encoder instead to drive for a certain distance rather than 
          // a certain amount of time.
          
          // It may be that when it entered the if statement, the robot has reached the end of the stage, but 
          // hasn't gotten far enough into the 
          // stage's "endzone" for nine sensors to turn white. To account for this the following two lines of code
          // should be looped through WHILE it is driving foward briefly (see code directly above). (This is not currently 
          // the case; instead these two lines are currently being run AFTER it has driven forward briefly.) Once
          // "delay(150)" above is replaced with a loop that checks the encoder, these two lines of code can simply 
          // be inserted into the loop (with adaptations made as necessary).                  
          num_white = count_white(color);
          if(num_white > 9) break;
          
          pos = position();
          if (pos < 6) turn(1);  // Turn right.
          else turn(-1);
          // Reset PID controller signals.
          last_error = 0;
          total_error = 0;
          prev_line_pos = 6;

        }
        follow_line();
      } 
      
      break;

    case 2:
      
      // For now just stop.
      M1_stop();
      M2_stop();
    
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
