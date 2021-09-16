#ifndef TERRARIUM_DISPLAY
#define TERRARIUM_DISPLAY

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "telemetry.h"
#include "config.h"

namespace Display
{
     void displaySetup();
     void renderClimate(Telemetry::TelemteryData telemteryData);
     void renderConnectingToWifi(char *ssid, int attempts);
     void renderNtp(int attempts);
     void renderConnectedToWifi(char* ssid);
     void renderTime(int hour, int minute, int second);
     void renderHarvestInfo(int secondsToNextHarvest);
     char * floatToString(double value);
}
#endif