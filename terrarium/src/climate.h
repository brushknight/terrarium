#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

namespace Climate
{

    struct climateData;

    void climateSetup();
    climateData readTempHumid(DHT_Unified dht);
    void turnRelayOn();
    void turnRelayOff();
    void climateControl(int hour, int minute);
}