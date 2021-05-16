#include "main.h"

using namespace Climate;
using namespace Lighting;
using namespace Security;
using namespace Net;
using namespace RealTime;
using namespace Telemetry;

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
  TelemteryData telemteryData = climateControl(getHour(), getMinute());
  // securityCheck();

  send(telemteryData);

  delay(delayMS);
}
