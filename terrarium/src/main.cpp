#include "main.h"

using namespace Climate;
using namespace Lighting;
using namespace Security;
using namespace Net;
using namespace RealTime;

uint32_t delayMS = 1000;


#define STATUS_LED 2


void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  connect();
  syncTime();
  printLocalTime();

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

  digitalWrite(STATUS_LED, 1);

  delay(delayMS);
}
