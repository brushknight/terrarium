#include "real_time.h"

namespace RealTime
{

    const char *ntpServer1 = "pool.ntp.org";
    const char *ntpServer2 = "1.europe.pool.ntp.org";
    const char *ntpServer3 = "2.europe.pool.ntp.org";
    const long gmtOffset_sec = 0;     // todo fix this to be +1
    const int daylightOffset_sec = 0; // fix this to accept DST

    RTC_DS3231 rtc;

    void setupRtcModule()
    {
        if (!rtc.begin())
        {
            Serial.println("Couldn't find RTC!");
            Serial.flush();
            abort();
        }

        if (rtc.lostPower())
        {
            Serial.println("RTC: lost power");
            syncTime();

            struct tm timeinfo;
            int attempts = 0;
            while (!getLocalTime(&timeinfo))
            {
                attempts++;
                Serial.println("Failed to obtain time, retry");
                syncTime();
                Display::renderNtp(attempts);
                if (attempts >= 20)
                {
                    attempts = 0;
                }
            }

            rtc.adjust(mktime(&timeinfo));
        }

        printLocalTime();
        Serial.println("RTC: now");
        Serial.println(rtc.now().hour());
    }

    void syncTime()
    {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2, ntpServer3);
    }

    int getHour()
    {
        return int(rtc.now().hour());
    }

    int getMinute()
    {
        return int(rtc.now().minute());
    }

    int getSecond()
    {
        return int(rtc.now().second());
    }

    uint32_t getTimestamp()
    {
        return rtc.now().secondstime();
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
}
