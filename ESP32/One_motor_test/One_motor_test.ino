
#include <Wire.h>


#define BRAKE     26
#define DIR1      4
#define PWM1      32 
#define PWM1_CH   1 

#define TIMER_BIT 8
#define BASE_FREQ 20000  //20 KHz 

long currentT, previousT_1, previousT_2 = 0; 
int loop_time = 10;

void setup(){

  Serial.begin(9600); 
  pinMode(BRAKE,OUTPUT);
  pinMode(DIR1,OUTPUT);
  digitalWrite(BRAKE, HIGH);
  // Using LED control (LEDC) library for ESP32 to configure the frequency and resolution of an LEDC channel 
  ledcSetup(PWM1_CH, BASE_FREQ, TIMER_BIT);  
  // associate a GPIO pin with an LEDC channel 
  ledcAttachPin(PWM1, PWM1_CH);
  Motor1_control(0);
  delay(2000);

}


void loop() {

  currentT = millis(); 

  if (currentT - previousT_1 >= loop_time){

    digitalWrite(BRAKE, HIGH);
    Motor1_control(20);

    previousT_1 = currentT; 
  } 

}

void Motor1_control(int sp){
  if (sp < 0){
    digitalWrite(DIR1, LOW);
    sp = -sp;
  } else {
    digitalWrite(DIR1, HIGH);
  }
  pwmSet(PWM1_CH, sp > 255 ? 255 : 255 - sp);
}


void pwmSet(uint8_t channel, uint32_t value ){
  ledcWrite(channel, value); 
}





















