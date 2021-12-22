#ifndef TERRARIUM_STATUS
#define TERRARIUM_STATUS

#include <Adafruit_NeoPixel.h>

namespace Status
{
    void setup();
    void warning();
    void error();
    void ok();
    void connectingToWifi();
    void startHttpServer();
    void fetchTime();
    void turnLedOn(int r, int g, int b);
}

#endif