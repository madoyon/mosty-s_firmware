#include <Arduino.h>
#include "circular_buffer.h"
#include <Wire.h>


#define WIRE wire
// put function declarations here:
int myFunction(int, int);

uint8_t buffer[10] = {0};

circular_buffer_t* pcbuf;

void setup() {

  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  Serial.begin(115200);

  pcbuf = init_circular_buffer(buffer, 10);

  Wire.setPins(20, 21);
  Wire.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello world");

   byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(100);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}