#ifndef TERRARIUM_DISPLAY
#define TERRARIUM_DISPLAY

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "telemetry.h"
#include "config.h"

namespace Display
{

     class DisplayData
     {
     public:
          Climate::ClimateData hotSide;
          Climate::ClimateData hotCenter;
          Climate::ClimateData coldCenter;
          Climate::ClimateData coldSide;
          bool heater;
          Climate::HeaterPhase heaterPhase;
          int hour;
          int minute;
          int second;
          int nextHarvestInSec;
          int terrId;
          bool submission;
     };

     void displaySetup();
     void renderClimate(DisplayData displayData);
     void renderConnectingToWifi(char *ssid, int attempts);
     void renderNtp(int attempts);
     void renderInfo(int id);
     void renderConnectedToWifi(char *ssid);
     void renderTime(int hour, int minute, int second);
     void renderHarvestInfo(int secondsToNextHarvest);
     char *floatToString(double value);
     void clearRow(int row);
     void render(DisplayData displayData);
     void renderSubmissionInfo(bool submission);

}
#endif