#ifndef TERRARIUM_CLIMATE
#define TERRARIUM_CLIMATE

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "telemetry.h"
#include "climate_data.h"
#include "config.h"

namespace Climate
{

    void climateSetup(uint32_t now);
    ClimateData readTempHumid(DHT_Unified dht);
    void turnHotZoneRelayOn();
    void turnHotZoneRelayOff();
    void turnColdZoneRelayOn();
    void turnColdZoneRelayOff();
    Telemetry::TelemteryData climateControl(int hour, int minute, uint32_t now);
}

#endif