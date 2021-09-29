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
Telemetry::TelemteryData gTelemteryData = Telemetry::TelemteryData();

#define HARVESTING_INTERVAL_SEC 5
#define SUBMISSION_INTERVAL_SEC 30
#define DISPLAY_REFRESH_INTERVAL 1
#define DISPLAY_REINIT_INTERVAL 60 * 3

void submitTelemetry(void * parameter){
  for(;;){ // infinite loop

    Serial.println("submission started");
    displayData.submission = true;
    Telemetry::send(gTelemteryData);
    displayData.submission = false;
    Serial.println("submission finished");


    // Pause the task again for 500ms
    vTaskDelay(1000 * SUBMISSION_INTERVAL_SEC / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  Display::displaySetup();

  Display::bootScreen();

  //connect();
  RealTime::setupRtcModule();

  int now = RealTime::getTimestamp();

  //securitySetup();
  climateSetup(now);
  //ledSetup();

  //Encoder::setup();

  xTaskCreate(
    submitTelemetry,    // Function that should be called
    "submitTelemetry",   // Name of the task (for debugging)
    1024 * 10 ,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );

  delay(1000);

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
    telemteryData.hour = RealTime::getHour();
    telemteryData.minute = RealTime::getMinute();
    telemteryData.second = RealTime::getSecond();
    telemteryData.version = VERSION;
    telemteryData.uptime = esp_timer_get_time() / 1000000;

    gTelemteryData = telemteryData;
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

