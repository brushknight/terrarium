#include "main.h"

using namespace Climate;
using namespace Lighting;
using namespace Security;
using namespace Net;
using namespace RealTime;
using namespace Telemetry;
using namespace Display;

uint32_t delayMS = 1000;

void setup()
{
  Serial.begin(115200);
  displaySetup();
  
  connect();
  syncTime();
  printLocalTime();

  //securitySetup();
  climateSetup();
  //ledSetup();

}

void loop()
{
  // turnLedOn(255,0,0);
  TelemteryData telemteryData = climateControl(getHour(), getMinute());
  // securityCheck();

  //send(telemteryData);



  renderClimate(telemteryData);

  delay(delayMS);
}

/*
#include <Wire.h>
#include <Arduino.h>
 
void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
}
 
void loop() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);          
}

*/