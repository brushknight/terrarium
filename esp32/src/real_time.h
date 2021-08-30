#ifndef TERRARIUM_RTC
#define TERRARIUM_RTC

#include "time.h"
#include "Arduino.h"
#include <RTClib.h>

namespace RealTime{
    void syncTime();
    void setupRtcModule();
    void printLocalTime();
    int getHour();
    int getMinute();
}

#endif