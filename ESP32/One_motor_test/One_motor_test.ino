
#define BRAKE     16
#define DIR      17
#define PWM      4 
#define PWM_CH   1 

#define TIMER_BIT 8
#define BASE_FREQ 20000  //20 KHz 


void Motor_control(int sp){
  if (sp <0){
    digitalWrite(DIR, LOW); 
    sp = -sp; 
  }
  else {
    digitalWrite(DIR, HIGH); 
  }
  pwmSet(PWM_CH, sp > 255 ? 255 : 255 - sp); 
}


void pwmSet(uint8_t channel, uint32_t value){
  ledcWrite(channel, value); 
}



void setup(){

  Serial.begin(9600); 
  pinMode(BRAKE,OUTPUT);
  pinMode(DIR,OUTPUT);
  digitalWrite(BRAKE, HIGH);
  ledcSetup(PWM_CH, BASE_FREQ, TIMER_BIT);  
  ledcAttachPin(PWM, PWM_CH);
  Motor_control(0);
  delay(2000);

}


void loop() {
  Motor_control(8);
  delay(100); 
}

