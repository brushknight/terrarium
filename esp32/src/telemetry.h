#ifndef TERRARIUM_TELEMETRY
#define TERRARIUM_TELEMETRY

#include "climate_data.h"
#include "config.h"
#include "net.h"

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
        bool hotZoneHeater;
        bool coldZoneHeater;
        HeaterPhase hotZoneheaterPhase;
        HeaterPhase coldZoneheaterPhase;
        bool fullfilled;
        char* version;
        int uptime;
        int hour;
        int minute;
        int second;
        bool isDay;
    };

    void send(TelemteryData telemteryData);

}

#endif