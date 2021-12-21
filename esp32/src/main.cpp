#include "main.h"

uint32_t lastTimeReinit = 0;
Display::DisplayData displayData = Display::DisplayData();
Telemetry::TelemteryData gTelemteryData = Telemetry::TelemteryData();

static portMUX_TYPE display_mutex;

#define HARVESTING_INTERVAL_SEC 5
#define SUBMISSION_INTERVAL_SEC 30
#define DISPLAY_REFRESH_INTERVAL 1
#define DISPLAY_REINIT_INTERVAL 60 * 3
#define SAMPLE_DUPLICATIONS_COUNT_ALLOWED 3

void submitTelemetry(void *parameter)
{

  int lastSampleIdSent = 0;
  int sampleDuplicationSinceLastSent = 0;


  for (;;)
  {
    if (lastSampleIdSent != gTelemteryData.sampleId && lastSampleIdSent < gTelemteryData.sampleId)
    {
      Serial.println("submission started");
      displayData.submission = true;
      Telemetry::send(gTelemteryData);
      lastSampleIdSent = gTelemteryData.sampleId;
      displayData.submission = false;
      Serial.print("submission finished, sample ID: ");
      Serial.println(lastSampleIdSent);
    }
    else
    {
      Serial.println("sample already sent");
      gTelemteryData = Telemetry::TelemteryData();
      sampleDuplicationSinceLastSent++;
    }

    if(sampleDuplicationSinceLastSent > SAMPLE_DUPLICATIONS_COUNT_ALLOWED){
      ESP.restart();
    }

    // Pause before next submission interval
    vTaskDelay(1000 * SUBMISSION_INTERVAL_SEC / portTICK_PERIOD_MS);
  }
}

void statusLed(void *parameter)
{
  for (;;)
  {
    Lighting::turnLedOn(255,0,0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Lighting::turnLedOn(255,69,0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Lighting::turnLedOn(255,140,0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Lighting::turnLedOn(0,255,0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Lighting::turnLedOn(0,0,255);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void climateControl(void *parameter){

  int uptime = RealTime::getUptime();

  for (;;){
    uptime = RealTime::getUptime();
    Telemetry::TelemteryData telemteryData = Climate::control(RealTime::getHour(), RealTime::getMinute(), uptime);
    telemteryData.second = RealTime::getSecond();
    telemteryData.version = VERSION;

    gTelemteryData = telemteryData;

    displayData.hotSide = telemteryData.hotSide;
    displayData.hotCenter = telemteryData.hotCenter;
    displayData.coldCenter = telemteryData.coldCenter;
    displayData.coldSide = telemteryData.coldSide;
    displayData.hotZoneHeater = telemteryData.hotZoneHeater;
    displayData.coldZoneHeater = telemteryData.coldZoneHeater;
    displayData.hotZoneHeaterPhase = telemteryData.hotZoneHeaterPhase;
    displayData.coldZoneHeaterPhase = telemteryData.coldZoneHeaterPhase;


    vTaskDelay(HARVESTING_INTERVAL_SEC * 1000 / portTICK_PERIOD_MS);
  }
}

void renderDisplay(void *parameter){
  for (;;){
    Serial.println("renderDisplay");
    if (DISPLAY_ENABLED)
    {
      displayData.nextHarvestInSec = 0;

      displayData.terrId = TERRARIUM_ID;
      displayData.hour = RealTime::getHour();
      displayData.minute = RealTime::getMinute();
      displayData.second = RealTime::getSecond();
      //portENTER_CRITICAL(&display_mutex);
      Display::render(displayData);
      //portEXIT_CRITICAL(&display_mutex);
    }

    vTaskDelay(DISPLAY_REFRESH_INTERVAL * 1000 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);

  vPortCPUInitializeMutex(&display_mutex);

  if (DISPLAY_ENABLED)
  {
    Display::displaySetup();
    Display::bootScreen();
  }

  if (RTC_ENABLED)
  {
    RealTime::setupRtcModule();
  }
  else
  {
    RealTime::setupWithoutRTC();
  }

  int uptime = RealTime::getUptime();
  Climate::setup(uptime);

  xTaskCreatePinnedToCore(
      submitTelemetry,   
      "submitTelemetry", 
      1024 * 10,         
      NULL,              
      1,                 
      NULL,               
      0
  );

  Lighting::ledSetup();

  xTaskCreatePinnedToCore(
    statusLed,   
    "statusLed", 
    1024,         
    NULL,             
    2,                 
    NULL,               
    1
  );

  xTaskCreatePinnedToCore(
    renderDisplay,   
    "renderDisplay", 
    1024 * 10,         
    NULL,              
    2,                 
    NULL,               
    1
  );

  xTaskCreatePinnedToCore(
    climateControl,   
    "climateControl", 
    1024 * 10,         
    NULL,              
    3,                 
    NULL,               
    0
  );
}

void loop()
{
  int uptime = RealTime::getUptime();

  if (uptime - lastTimeReinit >= DISPLAY_REINIT_INTERVAL)
  {
    Display::displaySetup();
    lastTimeReinit = uptime;
  }

}
