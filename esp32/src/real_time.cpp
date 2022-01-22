#include "real_time.h"

namespace RealTime
{

    const char *ntpServer1 = "10.0.0.51";
    const char *ntpServer2 = "pool.ntp.org";
    const char *ntpServer3 = "1.europe.pool.ntp.org";
    const long gmtOffset_sec = 3600;  // todo fix this to be +1
    const int daylightOffset_sec = 0; // fix this to accept DST

    // setup(rtc_enabled)
    // syncFromNTP()
    // flashRTC(time)
    // syncFromRTC()
    // getTime()
    // getHour()
    // getMinute()
    // getSecond()
    // getDate()

    RTC_DS3231 rtc;

    void setup(bool rtcEnabled)
    {
        Serial.println("RealTime: setup started");

        if (rtcEnabled)
        {
            Serial.println("RealTime: RTC enalbed");
            if (!rtc.begin())
            {
                Serial.println("Couldn't find RTC, check wiring!");
                Serial.flush();
                syncFromNTP();
                //abort(); -> only if NTP time failed
                return;
            }

            // or rtc time is a shit
            if (!rtc.lostPower())
            {
                
                syncFromRTC();
            }
            else
            {
                Serial.println("RealTime: RTC lost power");
                syncFromNTP();
                // validate time here
                saveTimeToRTC();
            }
        }

        else
        {
            syncFromNTP();
        }
        Serial.println("RealTime: setup finished");
    }

    void syncFromRTC()
    {
        Serial.println("RealTime: sync from RTC");
        DateTime rtcDateTime = rtc.now();

        struct timeval tv;
        tv.tv_sec = rtcDateTime.secondstime() + SECONDS_FROM_1970_TO_2000;

        settimeofday(&tv, NULL);
    }

    void syncFromNTP()
    {
        Serial.println("RealTime: sync from NTP");
        Net::connect(true);
        struct tm timeinfo;
        int attempts = 0;
        while (!getLocalTime(&timeinfo))
        {
            Serial.println("Failed to obtain time, retry");
            configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2, ntpServer3);
            attempts++;
        }
    }

    bool saveTimeToRTC()
    {
        Serial.println("RealTime: saving time into RTC");
        struct tm timeinfo;
        if (getLocalTime(&timeinfo))
        {
            rtc.adjust(mktime(&timeinfo));
            return true;
        }
        else
        {
            return false;
        }
    }

    int getHour()
    {
        int hour = 0;

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Failed to obtain time");
            abort();
        }

        hour = timeinfo.tm_hour;

        return hour;
    }

    int getMinute()
    {
        int minute = 0;

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Failed to obtain time");
            abort();
        }

        minute = timeinfo.tm_min;

        return minute;
    }

    int getSecond()
    {
        int second = 0;

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Failed to obtain time");
            abort();
        }

        second = timeinfo.tm_sec;

        return second;
    }

    void printLocalTime()
    {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Failed to obtain time");
            return;
        }
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
        Serial.print("Day of week: ");
        Serial.println(&timeinfo, "%A");
        Serial.print("Month: ");
        Serial.println(&timeinfo, "%B");
        Serial.print("Day of Month: ");
        Serial.println(&timeinfo, "%d");
        Serial.print("Year: ");
        Serial.println(&timeinfo, "%Y");
        Serial.print("Hour: ");
        Serial.println(&timeinfo, "%H");
        Serial.print("Hour (12 hour format): ");
        Serial.println(&timeinfo, "%I");
        Serial.print("Minute: ");
        Serial.println(&timeinfo, "%M");
        Serial.print("Second: ");
        Serial.println(&timeinfo, "%S");

        Serial.println("Time variables");
        char timeHour[3];
        strftime(timeHour, 3, "%H", &timeinfo);
        Serial.println(timeHour);
        char timeWeekDay[10];
        strftime(timeWeekDay, 10, "%A", &timeinfo);
        Serial.println(timeWeekDay);
        Serial.println();
    }

    int getBatteryPercent()
    {

        float oneMilliVolt = 3300.0 / 4096.0;

        float full = 3000.0 / oneMilliVolt;

        float empty = 2000.0 / oneMilliVolt;

        float current = float(analogRead(33));

        // TODO add check for zero
        if (full - empty == 0)
        {
            return 0;
        }

        float percent = (current - empty) / (full - empty);

        // if (percent > 1)
        // {
        //     percent = 1;
        // }

        return int(percent * 100.0);
    }

    int getBatteryVoltage()
    {

        float oneMilliVolt = 3300.0 / 4096.0;

        return int(float(analogRead(33)) * oneMilliVolt);
    }

    int getUptimeSec()
    {
        return esp_timer_get_time() / 1000000;
    }
}
