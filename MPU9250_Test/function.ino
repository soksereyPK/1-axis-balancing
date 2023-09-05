

void angle_setup(){
  Wire.begin();  // Initializes the I2C communication 
  delay(100); 
  // write a value 0 to the PM register, which turns it on and set the clock source to internal oscillator 8Mhz
  writeTo(MPU6050, PWR_MGMT_1, 0);  

  // write a value of accSens shifted left by 3 bits to the accelerometer configuration register, which sets the full-scale range
  writeTo(MPU6050, ACCEL_CONFIG, accSens << 3);

  // write a value of gyroSens shifted left by 3 bits, which sets the full-scale range 
  writeTo(MPU6050, GYRO_CONFIG, gyroSens << 3);
  delay(100);

  for (int i=0; i<1024, i++){
    angle_calc();

    // use to calculate the average offset of the gyroscope
    GyZ_offset_sum += Gyz
    delay(5);
  }

  // This statement divides the value of GyZ_offset_sum by 1024 using a right shift operation
  // Which gives the average value of GyZ and assigns it to the variable GyZ_offset
  // That present the bias of the gyroscope
  GyZ_offset = GyZ_offset_sum >> 10; 
  digitalWrite(BUZZER, HIGH);
  delay(70);
  digitalWrite(BUZZER, LOW);
  delay(80);
  digitalWrite(BUZZER, HIGH);
  delay(70);
  digitalWrite(BUZZER, LOW);
  Serial.print("GyZ offset value = "); Serial.println(GyZ_offset);
}




void angle_calc() {

  // Start transmission to I2C address of 0x68
  Wire.beginTransmission(MPU6050);
  // write the value of 0x3B, which is the register address of ACCEL_XOUT_H (MSB)
  Wire.write(0x3B);                  
  // End transmission without releasing I2C bus, which allows a repeated start condition
  Wire.endTransmission(false);
  // Request 4 bytes of data and release the I2C bus after reading 
  Wire.requestFrom(MPU6050, 4, true);
  // Read two bytes from sensor and combines into a 16-bit integer value, assigned to X axis 
  AcX = Wire.read() << 8 | Wire.read(); 
  // Read two bytes from the sensor and combines into a 16-bit integer value, assigned to Y axis
  AcY = Wire.read() << 8 | Wire.read(); 

  Wire.beginTransmission(MPU6050);
  Wire.write(0x47);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050, 2, true);  
  GyZ = Wire.read() << 8 | Wire.read(); // Assigned to z axis

  // add offset the the actual value data
  AcX += AcX_offset;
  AcY += AcY_offset;  
  AcZ += AcZ_offset;
  GyZ -= GyZ_offset;

  // calculate robot angle around z-axis, loop_time, and a scaling factor of 1/1000/65.535
  // Which convert the gyroscope data from LSB to degree per second 
  robot_angle += Gyz * loop_time / 1000 / 65.536; 

  // calculate the acc_angle, which represents the angle of the sensor around the z-axis based on the acc data
  // By using atan2 function, which returns the arctangent of AcY/AcX in radians, and multiplying it by 57.2958, which converts radian to degrees 
  Acc_angle = atan2(AcY, -AcX) * 57.2958;  

  // Combines the robot_angle and acc_angle using a complementary filter
  // Which is a technique that blends the gyroscope and accelerometer data to get a more accurate and stable angle measurement
  // Gyro_amount is a parameter that determines how much weight is given to the gyroscope data versus the accelerometer data
  robot_angle = robot_angle * Gyro_amount + Acc_angle * (1.0 - Gyro_amount);

  // Set vertical variable to false if the absolute value of robot_angle is greater than 9 degrees, which means that the sensor is tilted too much
  if (abs(robot_angle) > 9) vertical = false;
  if (abs(robot_angle) < 0.3) vertical = true;


  //Serial.print("Angle: "); Serial.println(robot_angle);





}