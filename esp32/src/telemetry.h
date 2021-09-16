#ifndef TERRARIUM_TELEMETRY
#define TERRARIUM_TELEMETRY

#include "climate_data.h"
#include "config.h"

namespace Telemetry
{

    using namespace Climate;

    class ClimateConfig
    {
        public:
        float dayMaxTemp;
        float nightMaxTemp;
        float dayTempToleranceWarm;
        float dayTempToleranceCold;
        float nightTempToleranceWarm;
        float nightTempToleranceCold;
    };

    class TelemteryData
    {
    public:
        ClimateData hotSide;
        ClimateData hotCenter;
        ClimateData coldCenter;
        ClimateData coldSide;
        ClimateConfig climateConfig;
        bool heater;
        HeaterPhase heaterPhase;
    };

    void send(TelemteryData telemteryData);

}

#endif