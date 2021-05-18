#ifndef TERRARIUM_TELEMETRY
#define TERRARIUM_TELEMETRY

#include "climate_data.h"

namespace Telemetry
{

    using namespace Climate;

    class ClimateConfig
    {
        public:
        float dayMaxTemp;
        float nightMaxTemp;
        float dayTempTolerance;
        float nightTempTolerance;
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