#include "time.h"
#include "Arduino.h"

namespace RealTime{
    void syncTime();
    void printLocalTime();
    int getHour();
    int getMinute();
}