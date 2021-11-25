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

    void setup(uint32_t now);
    ClimateData readTempHumid(DHT_Unified dht);
    Telemetry::TelemteryData control(int hour, int minute, uint32_t now);
}

#endif