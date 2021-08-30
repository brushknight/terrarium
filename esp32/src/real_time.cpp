#include "real_time.h"

namespace RealTime
{

    const char *ntpServer = "pool.ntp.org";
    const long gmtOffset_sec = 0;
    const int daylightOffset_sec = 3600 * 2;

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
            if (!getLocalTime(&timeinfo))
            {
                Serial.println("Failed to obtain time");
                return;
            }

            // involve ntp here
            // this will adjust to the date and time at compilation
            rtc.adjust(mktime(&timeinfo));
        }

        Serial.println("RTC: now");
        Serial.println(rtc.now().hour());
    }

    void syncTime()
    {

        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        printLocalTime();

        Serial.print("time as int: ");
        Serial.print(getHour());
        Serial.print(":");
        Serial.println(getMinute());
    }

    int getHour()
    {

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Failed to obtain time");
            return 0;
        }

        char timeHour[3];
        strftime(timeHour, 3, "%H", &timeinfo);

        // Serial.print("Hour: ");
        // Serial.println(timeHour);

        return atoi(timeHour);
    }
    int getMinute()
    {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            Serial.println("Failed to obtain time");
            return 0;
        }

        char timeMinute[3];
        strftime(timeMinute, 3, "%M", &timeinfo);

        // Serial.print("Minute: ");
        // Serial.println(timeMinute);

        return atoi(timeMinute);
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
