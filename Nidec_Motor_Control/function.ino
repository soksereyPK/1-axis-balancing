


void setPWM(uint16_t dutyCycle) { // dutyCycle is a value between 0-ICR1
    OCR1A = dutyCycle;
}


void Motor_control(int pwm){
  if (pwm <= 0) {
    digitalWrite(DIRECTION, LOW);
    pwm = -pwm;
  } else {
    digitalWrite(DIRECTION, HIGH);
  }

  //For example, if you want to map a value from 0 to 255 to 0 to 100, you can write: map(value, 0, 255, 0, 100)
  // It maps the value of pwm from 0 to 255 to PWMVALUE to 0 using the map function.
  // This means that a higher value of pwm corresponds to a lower value of duty cycle, and vice versa. 
  // This is because the PWM mode used in this code is inverting mode, which means that the output is set on Compare Match and cleared at BOTTOM.
  // It passes the mapped value as an argument to the setPWM function, which sets the values of OCR1A and OCR1B registers to that value.
  // This means that the duty cycle of the PWM signal on pins OC1A and OC1B will be equal to that value divided by ICR1.
  setPWM(map(pwm, 0, 255, PWMVALUE, 0));

}