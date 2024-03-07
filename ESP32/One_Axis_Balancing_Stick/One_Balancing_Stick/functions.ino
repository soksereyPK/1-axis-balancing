

void writeTo(byte device, byte address, byte value) {
  Wire.beginTransmission(device);  // Initialize I2C communication
  Wire.write(address);  // Sends the register address to the device 
  Wire.write(value);   // Write the data value to specified register 
  Wire.endTransmission(true);  // completes the transmission and optionally send a stop condition indicated by true
}

void beep() {
  tone(BUZZER, 2000); // Generate a tone of 1000 Hz on the BUZZER pin
  delay(70);          // Wait for 70 milliseconds
  noTone(BUZZER);     // Stop generating the tone
  delay(80);          // Wait for an additional 80 milliseconds
}



void angle_setup() {
  Wire.begin();
  delay (100);
  writeTo(MPU6050, PWR_MGMT_1, 0);  //// Write 0 the power mode with enable internal 8MHz oscillator
  writeTo(MPU6050, ACCEL_CONFIG, accSens << 3); // Specifying output scaling of accelerometer
  writeTo(MPU6050, GYRO_CONFIG, gyroSens << 3); // Specifying output scaling of gyroscope
  delay (100);

  // Find the average of GyZ over 1024 interation 
  for (int i = 0; i < 1024; i++) {
    angle_calc();
    GyZ_offset_sum += GyZ;  // add the current value of GyZ to GyZ_offset_sum over interation of 1024
    delay(5);
  }
  GyZ_offset = GyZ_offset_sum >> 10;
  Serial.print("GyZ offset value = "); Serial.println(GyZ_offset);
  beep();
  beep();

}
  
  

void angle_calc() {
  
  Wire.beginTransmission(MPU6050);  // Initiates communication
  Wire.write(0x47);  // Send the register address to gyroscope
  Wire.endTransmission(false);  // end transmission but leave the connection active for data transmission. The 'false' indicated arduino should not release bus after ending transmission. 
  Wire.requestFrom(MPU6050, 2, true);  // request 6 bytes of gyro data (2 bytes for each)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  Wire.beginTransmission(MPU6050);
  Wire.write(0x3B);                  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050, 4, true); 
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)


  // add mpu6050 offset values
  AcX += AcX_offset;
  AcY += AcY_offset;  
  GyZ -= GyZ_offset;

  // Estimate the robot's angle in X direction using below formulas
  robot_angle += GyZ * loop_time / 1000 / 65.536;   
  Acc_angle = atan2(AcY, -AcX) * 57.2958;               // angle from acc. values * 57.2958 (deg/rad)
  // Sensor fusion or complimentart filter of gyro and accelerometer data in x direction 
  robot_angle = robot_angle * Gyro_amount + Acc_angle * (1.0 - Gyro_amount);

  //SerialBT.print("AngleX: "); SerialBT.print(angleX); SerialBT.print(" AngleY: "); SerialBT.println(angleY);  
  if (abs(robot_angle) > 9) vertical = false;
  if (abs(robot_angle) < 0.3) vertical = true;

}


void pwmSet(uint8_t channel, uint32_t value) {
  ledcWrite(channel, value);
}

void Motor1_control(int sp) {
  if (sp < 0) {
    digitalWrite(DIR1, LOW);
    sp = -sp;
  } else {
    digitalWrite(DIR1, HIGH);
  }
  pwmSet(PWM1_CH, sp > 255 ? 255 : 255 - sp);
}

// int Tuning() {
//   if (!SerialBT.available())  return 0;  // Check if there is any data availiable to read. If no data, the function return 0 indicating no tuning action was perfromed 
//   delay(1);
//   char param = SerialBT.read();               // get parameter byte
//   if (!SerialBT.available()) return 0;
//   char cmd = SerialBT.read();                 // get command byte
//   //SerialBT.flush();
//   switch (param) {
//     case 'p':
//       if (cmd == '+')    K1 += 1;
//       if (cmd == '-')    K1 -= 1;
//       printValues();
//       break;
//     case 'i':
//       if (cmd == '+')    K2 += 0.05;
//       if (cmd == '-')    K2 -= 0.05;
//       printValues();
//       break;
//     case 's':
//       if (cmd == '+')    K3 += 0.005;
//       if (cmd == '-')    K3 -= 0.005;
//       printValues();
//       break; 
//     case 'c':
//       if (cmd == '+' && !calibrating) {
//         calibrating = true;
//         SerialBT.println("calibrating on");
//       }
//       if (cmd == '-' && calibrating)  {
//         SerialBT.print("X: "); SerialBT.print(robot_angleX); SerialBT.print(" Y: "); SerialBT.println(robot_angleY);
//         if (abs(robot_angleX) < 10 && abs(robot_angleY) < 10) {
//           offsets.ID1 = 99;
//           offsets.X1 = robot_angleX;
//           offsets.Y1 = robot_angleY;
//           SerialBT.println("Vertex OK.");
//           save();
//         } else if (robot_angleX > -45 && robot_angleX < -25 && robot_angleY > -30 && robot_angleY < -10) {
//           offsets.ID2 = 99;
//           offsets.X2 = robot_angleX;
//           offsets.Y2 = robot_angleY;
//           SerialBT.println("First edge OK.");
//           save();
//         } else if (robot_angleX > 20 && robot_angleX < 40 && robot_angleY > -30 && robot_angleY < -10) {
//           offsets.ID3 = 99;
//           offsets.X3 = robot_angleX;
//           offsets.Y3 = robot_angleY;
//           SerialBT.println("Second edge OK.");
//           save();
//         } else if (abs(robot_angleX) < 15 && robot_angleY > 30 && robot_angleY < 50) {
//           offsets.ID4 = 99;
//           offsets.X4 = robot_angleX;
//           offsets.Y4 = robot_angleY;
//           SerialBT.println("Third edge OK.");
//           save();
//         } else {
//           SerialBT.println("The angles are wrong!!!");
//           beep();
//           beep();
//         }
//       }
//       break;              
//    }
//    return 1;
// }

// void printValues() {
//   SerialBT.print("K1: "); SerialBT.print(K1);
//   SerialBT.print(" K2: "); SerialBT.print(K2);
//   SerialBT.print(" K3: "); SerialBT.println(K3,4);
// }




