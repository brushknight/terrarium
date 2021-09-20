#include "main.h"

using namespace Climate;
using namespace Lighting;
using namespace Security;
using namespace Net;

uint32_t delayMS = 100;
uint32_t lastSensorFetch = 0;
uint32_t lastTimeRender = 0;
uint32_t lastTimeReinit = 0;
uint32_t lastSendingTime = 0;

#define HARVESTING_INTERVAL_SEC 5
#define SENDING_INTERVAL_SEC 30
#define DISPLAY_REFRESH_INTERVAL 1
#define DISPLAY_REINIT_INTERVAL 30

void setup()
{
  Serial.begin(115200);
  Display::displaySetup();

  connect();
  RealTime::setupRtcModule();

  //securitySetup();
  climateSetup();
  //ledSetup();

  Encoder::setup();
}

void loop()
{

  Encoder::tick();
  //Encoder::isTurn();
  int now = RealTime::getTimestamp();
  // turnLedOn(255,0,0);

  if (now - lastTimeReinit >= DISPLAY_REINIT_INTERVAL)
  {
    Display::displaySetup();
    lastTimeReinit = now;
  }

  if (now - lastTimeRender >= DISPLAY_REFRESH_INTERVAL)
  {
    Display::renderTime(RealTime::getHour(), RealTime::getMinute(), RealTime::getSecond());
    Display::renderHarvestInfo(HARVESTING_INTERVAL_SEC - (now - lastSensorFetch));
    Display::renderInfo(TERRARIUM_ID);
    lastTimeRender = now;
  }

  if (now - lastSensorFetch >= HARVESTING_INTERVAL_SEC)
  {
    Telemetry::TelemteryData telemteryData = climateControl(RealTime::getHour(), RealTime::getMinute());
    Display::renderClimate(telemteryData);
    lastSensorFetch = now;

    /*if (now - lastSendingTime >= SENDING_INTERVAL_SEC)
    {
      renderSendTelemetry();
      Telemetry::send(telemteryData);
      lastSendingTime = now;
    }*/
  }
}
