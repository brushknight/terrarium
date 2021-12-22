#include "status.h"

namespace Status
{

#define LEDPIN 23
#define LED_COUNT 1
#define LED_BRIGHTNESS 50

    int pixelFormat = NEO_GRB + NEO_KHZ800;

    Adafruit_NeoPixel *pixels;

    void setup()
    {
        pixels = new Adafruit_NeoPixel(LED_COUNT, LEDPIN, pixelFormat);
        pixels->begin();
        pixels->setBrightness(LED_BRIGHTNESS);
    }

    void warning() {
        turnLedOn(255,106,0);
    }
    void error() {
        turnLedOn(255,0,0);
    }
    void ok() {
         turnLedOn(84,22,180);
    }
    void connectingToWifi() {
        turnLedOn(0,181,236);
    }
    void startHttpServer() {
        turnLedOn(0,255,0);
    }
    void fetchTime() {
        turnLedOn(254,203,0);
    }

    void turnLedOn(int r, int g, int b)
    {

        pixels->setPixelColor(0, pixels->Color(r, g, b));
        pixels->show();
    }
}