
#define BUZZER      2

#define BRAKE       16
#define DIR1        17
#define PWM1        4
#define PWM1_CH     1

#define TIMER_BIT  8
#define BASE_FREQ  20000

#define MPU6050 0x68              // Device address
#define ACCEL_CONFIG 0x1C         // Accelerometer configuration address
#define GYRO_CONFIG  0x1B         // Gyro configuration address

#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C

//Sensor output scaling
#define accSens 0             // 0 = 2g, 1 = 4g, 2 = 8g, 3 = 16g
#define gyroSens 1            // 0 = 250rad/s, 1 = 500rad/s, 2 1000rad/s, 3 = 2000rad/s


float Gyro_amount = 0.996;  
bool vertical = false;

float K1 = 75;
float K2 = 5.25; 
float K3 = 0.04;
int loop_time = 10;

float alpha = 0.4;  
int pwm_s = 0; 

int16_t  AcX, AcY, AcZ, GyZ, gyroZ, gyroZfilt;
float robot_angle;
int32_t motor_speed;
int16_t  AcX_offset = 0;
int16_t  AcY_offset = 0;
int16_t  AcZ_offset = 0;
int16_t  GyZ_offset = 0;
int32_t  GyZ_offset_sum = 0;



float robot_angleX, robot_angleY, angleX, angleY;
float Acc_angle;      
  
long currentT, previousT_1; 