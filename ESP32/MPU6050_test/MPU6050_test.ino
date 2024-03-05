
#include <Wire.h>

#define MPU6050 0x68 // device address
#define ACCEL_CONFIG 0x1C   // Accelerometer configuration address 
#define GYRO_CONFIG 0x1B    // Gyro configuration address 


#define PWR_MGMT_1 0x6B   // Power Management Mode 
#define PWR_MGMT_2 0x6C 

//Sensor output scaling 
#define accSens 0   // 0 = 2g, 1 = 4g, 2 = 8g, 3 = 16g
#define gyroSens 1   // 0 = 250rad/s, 1 = 500rad/s, 2 1000rad/s, 3 = 2000rad/s

float Gyro_amount = 0.1;
int loop_time = 10;

int16_t  AcX, AcY, AcZ, GyX, GyY, GyZ, gyroX, gyroY, gyroZ, gyroYfilt, gyroZfilt;
int16_t  GyZ_offset = 0;
int16_t  GyY_offset = 0;
int16_t  GyX_offset = 0;
int32_t  GyZ_offset_sum = 0;
int32_t  GyY_offset_sum = 0;
int32_t  GyX_offset_sum = 0;

float robot_angleX, robot_angleY, angleX, angleY;
float Acc_angleX, Acc_angleY;      
int32_t motor_speed_X;  
int32_t motor_speed_Y;  

long currentT, previousT_1, previousT_2 = 0; 

void writeTo(byte device, byte address, byte value) {
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.write(value);
  Wire.endTransmission(true);
}


void angle_setup(){

  Wire.begin(); 
  delay(100);
  writeTo(MPU6050, PWR_MGMT_1, 0);  // Write 0 the power mode with enable internal 8MHz oscillator 
  writeTo(MPU6050, ACCEL_CONFIG, accSens << 3); // Specifying output scaling of accelerometer
  writeTo(MPU6050, GYRO_CONFIG, gyroSens << 3); // Specifying output scaling of gyroscope
  delay (100);

  for (int i = 0; i<1024; i++){
    angle_calc(); 
    GyZ_offset_sum += GyZ;    // add the current value of GyZ to GyZ_offset_sum over interation of 1024
    delay(3);
  }

  // Calculate the average gyro value of GyZ over the 1024 iterations reading by shifting 10 bits to the right which is qual to dividing by 1024
  GyZ_offset = GyZ_offset_sum >> 10;  
  Serial.print("GyZ offset value = "); Serial.println(GyZ_offset);

  for (int i = 0; i < 1024; i++) {
    angle_calc();
    GyY_offset_sum += GyY;
    delay(3);
  }
  GyY_offset = GyY_offset_sum >> 10;
  Serial.print("GyY offset value = "); Serial.println(GyY_offset);

  for (int i = 0; i < 1024; i++) {
    angle_calc();
    GyX_offset_sum += GyX;
    delay(3);
  }
  GyX_offset = GyX_offset_sum >> 10;
  Serial.print("GyX offset value = "); Serial.println(GyX_offset);

}


void angle_calc(){

  Wire.beginTransmission(MPU6050); // Initiates communication with the MPU6050 sensor using I2C
  Wire.write(0x43);   // Stating address to read data from gyroscope 
  Wire.endTransmission(false);  // end transmission but leaves the connection active for further data transmission. The 'false' indicated arduino should not release bus after ending transmission. 
  Wire.requestFrom(MPU6050, 6, true); // Start request 6 bytes of gyro data (2 bytes for each) 
  // Read gyro for x,y and z axis. Read 2 bytes (high byte and low byte) combines them to from 16 bit integer
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  Wire.beginTransmission(MPU6050);
  Wire.write(0x3B);                  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050, 6, true); 
  // The same as gyro read 2 bytes of data and combine to from 16 bit integer  
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  // add mpu6050 offset values
  GyZ -= GyZ_offset;   // substracting offset valus ensure the readings are more accurate and reliable 
  GyY -= GyY_offset;   // as they corrected inherent biases or drifts in the sensor 
  GyX -= GyX_offset;

  robot_angleX += GyZ * loop_time / 1000 / 65.536;   // Integrating gyroscope reading over time to update the estimated angle along x-axis. Devided by 65.536 covert to degree per second 
  Acc_angleX = atan2(AcY, -AcX) * 57.2958;           // angle from acc. values * 57.2958 (deg/rad)
  robot_angleX = robot_angleX * Gyro_amount + Acc_angleX * (1.0 - Gyro_amount); // Sensor fusion complementary filter 
 
  robot_angleY += GyY * loop_time / 1000 / 65.536; 
  Acc_angleY = -atan2(AcZ, -AcX) * 57.2958;              // angle from acc. values * 57.2958 (deg/rad)
  robot_angleY = robot_angleY * Gyro_amount + Acc_angleY * (1.0 - Gyro_amount);

  angleX = robot_angleX;
  angleY = robot_angleY;

  Serial.println(angleX); 

}



void setup() {
  Serial.begin(115200);
  angle_setup(); 

}

void loop() {

    currentT = millis();

  if (currentT - previousT_1 >= loop_time) {
    angle_calc();
  }

 


}
