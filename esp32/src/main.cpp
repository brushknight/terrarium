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
Display::DisplayData displayData = Display::DisplayData();

#define HARVESTING_INTERVAL_SEC 5
#define SENDING_INTERVAL_SEC 30
#define DISPLAY_REFRESH_INTERVAL 1
#define DISPLAY_REINIT_INTERVAL 60 * 3

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
  int now = RealTime::getTimestamp();  

  //Encoder::tick();
  //Encoder::isTurn();
  // turnLedOn(255,0,0);

  if (now - lastTimeReinit >= DISPLAY_REINIT_INTERVAL)
  {
    Display::displaySetup();
    lastTimeReinit = now;
  }

  if (now - lastSensorFetch >= HARVESTING_INTERVAL_SEC)
  {
    Telemetry::TelemteryData telemteryData = climateControl(RealTime::getHour(), RealTime::getMinute(), now);
    lastSensorFetch = now;

    displayData.hotSide = telemteryData.hotSide;
    displayData.hotCenter = telemteryData.hotCenter;
    displayData.coldCenter = telemteryData.coldCenter;
    displayData.coldSide = telemteryData.coldSide;
    displayData.heater = telemteryData.heater;
    displayData.heaterPhase = telemteryData.heaterPhase;

    /*if (now - lastSendingTime >= SENDING_INTERVAL_SEC)
    {
      renderSendTelemetry();
      Telemetry::send(telemteryData);
      lastSendingTime = now;
    }*/
  }

  displayData.nextHarvestInSec = HARVESTING_INTERVAL_SEC - (now - lastSensorFetch);

  displayData.terrId = TERRARIUM_ID;
  displayData.hour = RealTime::getHour();
  displayData.minute = RealTime::getMinute();
  displayData.second = RealTime::getSecond();

  if (now - lastTimeRender >= DISPLAY_REFRESH_INTERVAL)
  {
    Display::render(displayData);
    lastTimeRender = now;
  }
}
