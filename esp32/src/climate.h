#ifndef TERRARIUM_CLIMATE
#define TERRARIUM_CLIMATE

#include "DHTStable.h"
#include "thermistor.h"
#include "HardwareSerial.h"
#include "telemetry.h"
#include "climate_data.h"
#include "config.h"

namespace Climate
{

    void setup(uint32_t now);
    int readThermistor(int pin);
    ClimateData readSensor(int pin);
    Telemetry::TelemteryData control(int hour, int minute, uint32_t now);
    void enableSensors();
    void disableSensors();
}

#endif