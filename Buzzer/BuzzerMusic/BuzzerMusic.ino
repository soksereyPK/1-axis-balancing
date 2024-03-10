
#include "Buzzer_music.h"

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

long currentT, previousT_1;
float loop_time = 100;  

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32-Cube-blue"); //Bluetooth device name
//  harryPoter();
//  mario();
//  starwars();  
//  hbd(); 

}

void loop() {
  
  currentT = millis();

  if (currentT - previousT_1 >= loop_time) {
    Tuning(); 
    
    previousT_1 = currentT;

  }

}


void Tuning() {

  if (!SerialBT.available())  return;
  char param = SerialBT.read();

     switch (param) {
    case '1':
      harryPoter();
      break;
     case '2':
      mario();
      break;
     case '3':
      starwars();
      break;  
     case '4':
      hbd(); 
      break; 
  }
}


