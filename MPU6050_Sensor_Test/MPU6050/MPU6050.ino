
#include <Wire.h>

#define MPU6050       0x68         // Device address
#define ACCEL_CONFIG  0x1C         // Accelerometer configuration address
#define GYRO_CONFIG   0x1B         // Gyro configuration address
#define PWR_MGMT_1    0x6B
#define PWR_MGMT_2    0x6C

#define BUZZER        12

float X1 = 75.0; 
float X2 = 5.25;   
float X3 = 0.04;  
float loop_time = 10; 

uint32_t timer;
long currentT, previousT_1, previousT_2 = 0; 
int16_t AcX, AcY, AcZ, GyZ, gyroZ;


//Sensor output scaling
#define accSens 0             // 0 = 2g, 1 = 4g, 2 = 8g, 3 = 16g
#define gyroSens 1            // 0 = 250rad/s, 1 = 500rad/s, 2 1000rad/s, 3 = 2000rad/s
#define Gyro_amount 0.996     // percent of gyro in complementary filter


//IMU offset values
int16_t  AcX_offset = -750;
int16_t  AcY_offset = 360;
int16_t  AcZ_offset = 0;
int16_t  GyZ_offset = 0;
int32_t  GyZ_offset_sum = 0;

float alpha = 0.40; 
float gyroZfilt;

float robot_angle;
float Acc_angle;

bool vertical = false; 


void setup() {
  Serial.begin(115200);
  angle_setup();
}

void loop() {
  currentT = millis();
  if (currentT - previousT_1 >= loop_time) {
    angle_calc();
    if (vertical) {
     // digitalWrite(BRAKE, HIGH);
      gyroZ = GyZ / 131.0; // Convert to deg/s
      
      gyroZfilt = alpha * gyroZ + (1 - alpha) * gyroZfilt;
     // pwm_s = -constrain(X1 * robot_angle + X2 * gyroZfilt + X3 * -motor_speed, -255, 255); 
     // motor_speed += pwm_s;
     // Serial.println(motor_speed);
    }else {
     // motor_speed = 0;
    }

    previousT_1 = currentT;

  }
}


















