
#include "ESP32.h"
#include <Wire.h>
#include "BluetoothSerial.h"
#include "Buzzer_music.h"

BluetoothSerial SerialBT;



void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32-Cube-blue"); 
  pinMode(BUZZER, OUTPUT);
  pinMode(BRAKE, OUTPUT);
  digitalWrite(BRAKE, HIGH);

  pinMode(DIR1, OUTPUT);
  ledcSetup(PWM1_CH, BASE_FREQ, TIMER_BIT);
  ledcAttachPin(PWM1, PWM1_CH);
  Motor1_control(0);
  delay(2000);

  angle_setup();
}

void loop() {

  currentT = millis();

  if (currentT - previousT_1 >= loop_time) {
    Tuning(); 

    previousT_1 = currentT;
    angle_calc();

    if (vertical) {
      digitalWrite(BRAKE, HIGH);
      gyroZ = GyZ / 131.0; // Convert to deg/s
      // Serial.print(angleX);
      // Serial.print(" , ");
      // Serial.println(gyroZ);
      gyroZfilt = alpha * gyroZ + (1 - alpha) * gyroZfilt;  // low pass filter 
      // Serial.print(gyroZ);
      // Serial.print(" , ");
      // Serial.println(gyroZfilt);
      pwm_s = -constrain(K1 * robot_angle + K2 * gyroZfilt + K3 * -motor_speed, -255, 255); 
      Serial.println(pwm_s);

      Motor1_control(pwm_s);
      motor_speed += pwm_s;
      
    }
    else {
      Motor1_control(0);
      digitalWrite(BRAKE, LOW);
      motor_speed = 0;

    }

  
  }
}

