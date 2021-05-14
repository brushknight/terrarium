#include "main.h"

using namespace Climate;
using namespace Lighting;
using namespace Security;
using namespace Net;
using namespace RealTime;

uint32_t delayMS = 1000;

void setup()
{
  Serial.begin(9600);

  connect();
  syncTime();
  printLocalTime();

  // securitySetup();
  climateSetup();
  // ledSetup();
}

void loop()
{

  // turnLedOn(255,0,0);
  climateControl(getHour(), getMinute());
  // securityCheck();

  delay(delayMS);
}
