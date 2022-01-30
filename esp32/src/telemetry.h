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
        int sampleId;
        ClimateData hotSide;
        ClimateData hotCenter;
        ClimateData coldCenter;
        ClimateData coldSide;
        ClimateConfig climateConfig;
        bool hotZoneHeater;
        bool coldZoneHeater;
        HeaterPhase hotZoneHeaterPhase;
        HeaterPhase coldZoneHeaterPhase;
        bool fullfilled;
        char *version;
        int uptime;
        int hour;
        int minute;
        int second;
        bool isDay;
    };

    void handlePrometheousEndpoint(WiFiClient client, TelemteryData gTelemteryData);
    void send(TelemteryData telemteryData);

}

#endif