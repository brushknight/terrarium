#include "main.h"

using namespace Climate;
using namespace Lighting;
using namespace Security;

uint32_t delayMS = 1000;


void setup() {
  Serial.begin(9600);

  securitySetup();
  climateSetup();
  ledSetup();


  // turnRelayOn();
  // delay(1000);
  // turnRelayOff();
  // delay(1000);


  // turnRelayOn();
  // delay(1000);
  // turnRelayOff();
  // delay(1000);

}

void loop() {
  
  turnLedOn(255,0,0);
  climateControl();
  securityCheck();

  delay(delayMS);
}
