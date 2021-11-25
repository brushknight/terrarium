#include "main.h"

using namespace Lighting;
using namespace Security;
using namespace Net;

uint32_t delayMS = 100;
uint32_t lastSensorFetch = 0;
uint32_t lastTimeRender = 0;
uint32_t lastTimeReinit = 0;
uint32_t lastSendingTime = 0;
Display::DisplayData displayData = Display::DisplayData();
Telemetry::TelemteryData gTelemteryData = Telemetry::TelemteryData();

#define HARVESTING_INTERVAL_SEC 5
#define SUBMISSION_INTERVAL_SEC 30
#define DISPLAY_REFRESH_INTERVAL 1
#define DISPLAY_REINIT_INTERVAL 60 * 3

void submitTelemetry(void *parameter)
{
  for (;;)
  { // infinite loop

    Serial.println("submission started");
    displayData.submission = true;
    Telemetry::send(gTelemteryData);
    displayData.submission = false;
    Serial.println("submission finished");

    // Pause before next submission interval
    vTaskDelay(1000 * SUBMISSION_INTERVAL_SEC / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);

  if (DISPLAY_ENABLED)
  {
    Display::displaySetup();
    Display::bootScreen();
  }

  //connect();
  if (RTC_ENABLED)
  {
    RealTime::setupRtcModule();
  }else{
    RealTime::setupWithoutRTC();
  }

  //int now = RealTime::getTimestamp();
  int uptime = RealTime::getUptime();
  //securitySetup();
  Climate::setup(uptime);
  //ledSetup();

  //Encoder::setup();

  xTaskCreate(
      submitTelemetry,   // Function that should be called
      "submitTelemetry", // Name of the task (for debugging)
      1024 * 10,         // Stack size (bytes)
      NULL,              // Parameter to pass
      1,                 // Task priority
      NULL               // Task handle
  );

  delay(1000);
}

void loop()
{
  int uptime = RealTime::getUptime();

  //Encoder::tick();
  //Encoder::isTurn();
  // turnLedOn(255,0,0);

  if (uptime - lastTimeReinit >= DISPLAY_REINIT_INTERVAL)
  {
    Display::displaySetup();
    lastTimeReinit = uptime;
  }

  if (uptime - lastSensorFetch >= HARVESTING_INTERVAL_SEC)
  {
    Telemetry::TelemteryData telemteryData = Climate::control(RealTime::getHour(), RealTime::getMinute(), uptime);
    telemteryData.hour = RealTime::getHour();
    telemteryData.minute = RealTime::getMinute();
    telemteryData.second = RealTime::getSecond();
    telemteryData.version = VERSION;
    telemteryData.uptime = uptime;

    gTelemteryData = telemteryData;
    lastSensorFetch = uptime;

    displayData.hotSide = telemteryData.hotSide;
    displayData.hotCenter = telemteryData.hotCenter;
    displayData.coldCenter = telemteryData.coldCenter;
    displayData.coldSide = telemteryData.coldSide;
    displayData.hotZoneHeater = telemteryData.hotZoneHeater;
    displayData.coldZoneHeater = telemteryData.coldZoneHeater;
    displayData.hotZoneHeaterPhase = telemteryData.hotZoneHeaterPhase;
    displayData.coldZoneHeaterPhase = telemteryData.coldZoneHeaterPhase;
    /*if (now - lastSendingTime >= SENDING_INTERVAL_SEC)
    {
      renderSendTelemetry();
      Telemetry::send(telemteryData);
      lastSendingTime = now;
    }*/
  }

  if (DISPLAY_ENABLED)
  {
    displayData.nextHarvestInSec = HARVESTING_INTERVAL_SEC - (uptime - lastSensorFetch);

    displayData.terrId = TERRARIUM_ID;
    displayData.hour = RealTime::getHour();
    displayData.minute = RealTime::getMinute();
    displayData.second = RealTime::getSecond();

    if (uptime - lastTimeRender >= DISPLAY_REFRESH_INTERVAL)
    {
      Display::render(displayData);
      lastTimeRender = uptime;
    }
  }
}
