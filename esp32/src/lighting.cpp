#include "lighting.h"

namespace Lighting
{
#define LEDPIN 14
#define LED_COUNT 1

    int pixelFormat = NEO_GRB + NEO_KHZ800;

    Adafruit_NeoPixel *pixels;

    void ledSetup()
    {
        pixels = new Adafruit_NeoPixel(LED_COUNT, LEDPIN, pixelFormat);
        pixels->begin();
    }

    void turnLedOn(int r, int g, int b)
    {
        pixels->clear();

        for (int i = 0; i < LED_COUNT; i++)
        {
            pixels->setPixelColor(i, pixels->Color(r, g, b));
            pixels->show();
        }
    }

}

// define led control pin

// function to enable lights
// function to disable lights