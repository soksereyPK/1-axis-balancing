
//Motor1 pins setup 
#define BRAKE         8 
#define PWM           9
#define DIRECTION     4


// PWM setup 
const uint16_t PWM_FREQUENCY = 20000;     // pwm frequency of Nidec 24h motor up to 20Khz
const uint16_t PWMVALUE = F_CPU / PWM_FREQUENCY / 2;   // Determines how many CPU cycles are in one period of the PWM signal 


// defined global variable to use latter 
int pwm_s = 0;
int32_t motor_speed; 
byte dir; 
uint32_t timer;
long currentT, previousT_1, previousT_2 = 0; 
float loop_time = 10;



void setup(){
  Serial.begin(115200);

  // Set PWM frequency to 20kHz - see the datasheet http://www.atmel.com/Images/doc8272.pdf page 128-135 
  // Set up PWM, Phase and Frequency Correct on pin 9 (OC1A) & pin 10 (OC1B) with ICR1 as TOP using Timer1
  TCCR1B = (1 << WGM13) | (1 << CS10);  // Set PWM Phase and Frequency Correct with ICR1 as TOP and no prescaling
  ICR1 = PWMVALUE;                      // ICR1 is the TOP value - this is set so the frequency is equal to 20kHz

  // Clear OC1A/OC1B on compare match when up-counting - Set OC1A/OC1B on compare match when downcounting
  TCCR1A = (1 << COM1A1) | (1 << COM1B1);
  setPWM(400);


  pinMode(PWM, OUTPUT);
  pinMode(BRAKE, OUTPUT);
  pinMode(DIRECTION, OUTPUT);
  digitalWrite(BRAKE, HIGH);
  delay(1000);


}

void loop (){

  currentT = millis();
  if (currentT - previousT_1 >= loop_time) 
    {

      Motor_control(pwm_s);

    }
  previousT_1 = currentT;

}






