
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

long currentT, previousT_1;
float loop_time = 100;  

float K1 = 160;
float K2 = 10.50; 
float K3 = 0.03;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SerialBT.begin("ESP32-Cube-blue"); //Bluetooth device name
}

void loop() {

   currentT = millis();
  if (currentT - previousT_1 >= loop_time) {
    Tuning(); 
    previousT_1 = currentT;
  }

}


int Tuning() {
  if (!SerialBT.available())  return 0;  // Check if there is any data available to read. If there isn't no data exits the function and return 0. 
  char param = SerialBT.read();  // Read single byte of data from BT and store in param 
  if (!SerialBT.available()) return 0; // Check again is there is any data available. No data exits and return 0. 
  char cmd = SerialBT.read();  // reads another byte of data and store in cmd 
  //SerialBT.flush();  // Clear serial buffer of any remaining data before next cycle
   switch (param) {
    case 'p':
      if (cmd == '+')    K1 += 1;
      if (cmd == '-')    K1 -= 1;
      printValues();
      break;
     case 'i':
      if (cmd == '+')    K2 += 0.01;
      if (cmd == '-')    K2 -= 0.01;
      printValues();
      break;
     case 's':
      if (cmd == '+')    K3 += 0.005;
      if (cmd == '-')    K3 -= 0.005;
      printValues();
      break;  
  }
}

void printValues() {
  SerialBT.print("K1: "); SerialBT.print(K1);
  SerialBT.print(" K2: "); SerialBT.print(K2);
  SerialBT.print(" K3: "); SerialBT.println(K3,4);
}

