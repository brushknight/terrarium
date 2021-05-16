#ifndef TERRARIUM_TELEMETRY
#define TERRARIUM_TELEMETRY

#include "climate_data.h"


namespace Telemetry
{


    using namespace Climate;

    class TelemteryData
    {
    public:
        ClimateData hotSide;
        ClimateData hotCenter;
        ClimateData coldCenter;
        ClimateData coldSide;
    };

    void send(TelemteryData telemteryData);

}

#endif