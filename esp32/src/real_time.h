#ifndef TERRARIUM_RTC
#define TERRARIUM_RTC

#include "time.h"
#include "Arduino.h"
#include <RTClib.h>
#include "display.h"
#include "net.h"

namespace RealTime{
    void syncTimeWithNTP();
    void setupRtcModule();
    void setupWithoutRTC();
    int getHour();
    int getMinute();
    int getSecond();
    void printLocalTime();
    int getUptime();
    int getBatteryPercent();
    int getBatteryVoltage();
}

#endif