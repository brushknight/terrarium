#ifndef TERRARIUM_DISPLAY
#define TERRARIUM_DISPLAY

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "telemetry.h"

namespace Display
{
     void displaySetup();
     void renderClimate(Telemetry::TelemteryData telemteryData);
     void renderConnectingToWifi(char* ssid);
     void renderConnectedToWifi(char* ssid);
}
#endif