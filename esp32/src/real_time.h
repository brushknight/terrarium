#ifndef TERRARIUM_RTC
#define TERRARIUM_RTC

#include "time.h"
#include "Arduino.h"
#include <RTClib.h>
#include "display.h"

namespace RealTime{
    void syncTime();
    void setupRtcModule();
    int getHour();
    int getMinute();
    int getSecond();
    void printLocalTime();
    uint32_t getTimestamp();
}

#endif