#ifndef TERRARIUM_RTC
#define TERRARIUM_RTC

#include "time.h"
#include "Arduino.h"
#include <RTClib.h>
#include "display.h"
#include "net.h"

namespace RealTime{
    void syncTime();
    void setupRtcModule();
    void setupWithoutRTC();
    int getHour();
    int getMinute();
    int getSecond();
    void printLocalTime();
    uint32_t getTimestamp();
    int getUptime();
}

#endif