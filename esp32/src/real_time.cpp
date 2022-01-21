#include "real_time.h"

namespace RealTime
{

    const char *ntpServer1 = "10.0.0.51";
    const char *ntpServer2 = "pool.ntp.org";
    const char *ntpServer3 = "1.europe.pool.ntp.org";
    const long gmtOffset_sec = 3600;  // todo fix this to be +1
    const int daylightOffset_sec = 0; // fix this to accept DST

    RTC_DS3231 rtc;

    int getUptime()
    {
        return esp_timer_get_time() / 1000000;
    }

    void setupRtcModule()
    {
        Serial.println("setupRtcModule");

        if (!rtc.begin())
        {
            Serial.println("Couldn't find RTC!");
            Serial.flush();
            abort();
        }

        Serial.print("RTC - power status: ");
        Serial.println(!rtc.lostPower());

        if (rtc.lostPower())
        {

            //Display::renderNtp(0);

            Serial.println("RTC: lost power");
            syncTimeWithNTP();

            struct tm timeinfo;
            int attempts = 0;
            while (!getLocalTime(&timeinfo))
            {
                attempts++;
                Serial.println("Failed to obtain time, retry");
                syncTimeWithNTP();
                //Display::renderNtp(attempts);
                if (attempts >= 20)
                {
                    attempts = 0;
                }
            }

            rtc.adjust(mktime(&timeinfo));
        }

        DateTime rtcDateTime = rtc.now();

        struct timeval tv;
        tv.tv_sec = rtcDateTime.secondstime();

        settimeofday(&tv, NULL);


        Serial.println("setupRtcModule finished");
    }

    void setupWithoutRTC()
    {
        syncTimeWithNTP();
    }

    void syncTimeWithNTP()
    {
        Net::connect(true);
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2, ntpServer3);
    }

    int getHour()
    {
        int hour = 0;

        if (RTC_ENABLED)
        {
            hour = int(rtc.now().hour());

            if (hour > 23)
            {
                syncTimeWithNTP();
                hour = int(rtc.now().hour());
            }
        }
        else
        {
            struct tm timeinfo;
            if (!getLocalTime(&timeinfo))
            {
                Serial.println("Failed to obtain time");
                abort();
            }

            hour = timeinfo.tm_hour;
        }

        return hour;
    }

    int getMinute()
    {
        int minute = 0;
        if (RTC_ENABLED)
        {
            minute = int(rtc.now().minute());
        }
        else
        {
            struct tm timeinfo;
            if (!getLocalTime(&timeinfo))
            {
                Serial.println("Failed to obtain time");
                abort();
            }

            minute = timeinfo.tm_min;
        }

        return minute;
    }

    int getSecond()
    {
        int second = 0;
        if (RTC_ENABLED)
        {
            second = int(rtc.now().second());
        }
        else
        {
            struct tm timeinfo;
            if (!getLocalTime(&timeinfo))
            {
                Serial.println("Failed to obtain time");
                abort();
            }

            second = timeinfo.tm_sec;
        }

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
        if (full - empty == 0){
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
}
