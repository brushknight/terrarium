#ifndef TERRARIUM_CLIMATE_DATA
#define TERRARIUM_CLIMATE_DATA

namespace Climate
{

    class ClimateData
    {
    public:
        float t;
        float h;
        int status;
    };

    enum HeaterPhase
    {
        heating,
        cooling
    };
}

#endif