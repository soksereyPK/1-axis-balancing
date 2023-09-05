
#include <Wire.h>

#define MPU6050       0x68         // Device address
#define ACCEL_CONFIG  0x1C         // Accelerometer configuration address
#define GYRO_CONFIG   0x1B         // Gyro configuration address
#define PWR_MGMT_1    0x6B
#define PWR_MGMT_2    0x6C


#define BRAKE         8 
#define PWM           9
#define DIRECTION     4

#define BUZZER        12
#define VBAT          A7

const uint16_t PWM_FREQUENCY = 20000;                
const uint16_t PWMVALUE = F_CPU / PWM_FREQUENCY / 2;


int pwm_s = 0;
byte dir;
int32_t motor_speed; 
uint32_t timer;
long currentT, previousT_1, previousT_2 = 0; 
int16_t AcX, AcY, AcZ, GyZ, gyroZ;

float X1 = 75.0; 
float X2 = 5.25;   
float X3 = 0.04;  
float loop_time = 10;


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
uint8_t i2cData[14]; // Buffer for I2C data


void setup() {
  Serial.begin(115200);
  TCCR1B = (1 << WGM13) | (1 << CS10);  
  ICR1 = PWMVALUE;   
  TCCR1A = (1 << COM1A1) | (1 << COM1B1);
  setPWM(400);
  Serial.print("PWM: "); Serial.println(PWMVALUE); Serial.print("CPU_FREQ: "); Serial.println(F_CPU);
  pinMode(PWM, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  pinMode(BRAKE, OUTPUT);
  pinMode(DIRECTION, OUTPUT);
  digitalWrite(BRAKE, HIGH);
  delay(1000);
  angle_setup();
}

void loop() {
  currentT = millis();
  if (currentT - previousT_1 >= loop_time){
    Tuning();
    angle_calc();
    if (vertical) {
      digitalWrite(BRAKE, HIGH);
      gyroZ = GyZ / 131.0;       //convert gyro data to deg/s

      // Applies a low-pass filter to the variable gyroZ, which reduces the noise and smooths the data.
      // alpha is a parameter that determines how much weight is given to the current data versus the previous data
      gyroZfilt = alpha * gyroZ + (1 - alpha) * gyroZfilt;

      // calculates the variable pwm_s, which represents the PWM signal for controlling the motor speed.
      // pwm_s is calculate by using a PID controller
      // The constrain function limits the value of pwm_s between -255 and 255
      pwm_s = -constrain(X1 * robot_angle + X2 * gyroZfilt + X3 * -motor_speed, -255, 255); 
      Motor_control(pwm_s);
      motor_speed += pwm_s;
    }
    else {
      Motor_control(0);
      digitalWrite(BRAKE, LOW);
      motor_speed = 0;
    }

  previousT_1 = currentT;
}

//   if (currentT - previousT_2 >= 500) {
//     battVoltage((double)analogRead(VBAT) / 53.7);  // This is then connected to a 47k-12k voltage divider
//     previousT_2 = currentT;
//   }
// }
