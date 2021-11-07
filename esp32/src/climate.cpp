#include "climate.h"

namespace Climate
{

    /* schedule

TODO: add 2 hour difference for UTC timezone

time in UTC

06 - 18 hot max 29.5
18 - 6 hot max 25 

*/

#define DAY_START_HOUR 6
#define DAY_START_MINUTE 0

#define NIGHT_START_HOUR 18
#define NIGHT_START_MINUTE 0

#define DHTTYPE DHT22
#define HEATER_RELAY_PIN 4

#define DHT_HOT_SIDE_PIN 16    // #1
#define DHT_HOT_CENTER_PIN 17  // #2
#define DHT_COLD_CENTER_PIN 18 // #3
#define DHT_COLD_SIDE_PIN 19   // #4

#define DAY_MAX_TEMP 28.5
#define DAY_TEMP_TOLERANCE_WARM 0.5
#define DAY_TEMP_TOLERANCE_COLD 0.6
#define NIGHT_MAX_TEMP 23
#define NIGHT_TEMP_TOLERANCE_WARM 0.5
#define NIGHT_TEMP_TOLERANCE_COLD 0.6

#define MAX_NULL_READINGS_SEC 30

    int lastNotNullReadings = 0;

    DHT_Unified dhtHotSide(DHT_HOT_SIDE_PIN, DHTTYPE);
    DHT_Unified dhtHotCenter(DHT_HOT_CENTER_PIN, DHTTYPE);
    DHT_Unified dhtColdCenter(DHT_COLD_CENTER_PIN, DHTTYPE);
    DHT_Unified dhtColdSide(DHT_COLD_SIDE_PIN, DHTTYPE);

    HeaterPhase heaterPhase;

    ClimateData readTempHumid(DHT_Unified dht)
    {
        ClimateData data;

        sensors_event_t event;
        dht.temperature().getEvent(&event);
        if (isnan(event.temperature))
        {
            Serial.println(F("Error reading temperature!"));
        }
        else
        {
            data.t = event.temperature;
            Serial.print(F("Temperature: "));
            Serial.print(event.temperature);
            Serial.println(F("°C"));
        }
        // Get humidity event and print its value.
        dht.humidity().getEvent(&event);
        if (isnan(event.relative_humidity))
        {
            Serial.println(F("Error reading humidity!"));
        }
        else
        {
            data.h = event.relative_humidity;
            Serial.print(F("Humidity: "));
            Serial.print(event.relative_humidity);
            Serial.println(F("%"));
        }
        return data;
    }

    void turnRelayOn()
    {
        heaterPhase = heating;
        if (OLD_SCHEME)
        {
            digitalWrite(HEATER_RELAY_PIN, LOW); // for old scheme
        }
        else
        {
            digitalWrite(HEATER_RELAY_PIN, HIGH);
        }

        //Serial.println("turn relay on");
    }

    void turnRelayOff()
    {
        heaterPhase = cooling;
        if (OLD_SCHEME)
        {
            digitalWrite(HEATER_RELAY_PIN, HIGH); // for old scheme
        }
        else
        {
            digitalWrite(HEATER_RELAY_PIN, LOW);
        }
        //Serial.println("turn relay off");
    }

    void climateSetup(uint32_t uptime)
    {
        pinMode(HEATER_RELAY_PIN, OUTPUT);
        turnRelayOff();
        dhtHotSide.begin();
        dhtHotCenter.begin();
        dhtColdCenter.begin();
        dhtColdSide.begin();
        lastNotNullReadings = uptime;
    }

    Telemetry::TelemteryData climateControl(int hour, int minute, uint32_t uptime)
    {

        bool isDay = hour >= DAY_START_HOUR && hour < NIGHT_START_HOUR && minute >= DAY_START_MINUTE;

        // Serial.print("is day: ");
        // Serial.println(isDay);

        // Serial.println("1: hot side");
        ClimateData hotSide = readTempHumid(dhtHotSide);
        // Serial.println("2: hot center");
        ClimateData hotCenter = readTempHumid(dhtHotCenter);
        // Serial.println("3: cold center");
        ClimateData coldCenter = readTempHumid(dhtColdCenter);
        // Serial.println("4: cold side");
        ClimateData coldSide = readTempHumid(dhtColdSide);

        if ((hotSide.t > 0 || hotCenter.t > 0) && (coldCenter.t > 0 || coldSide.t > 0))
        {
            lastNotNullReadings = uptime;
        }

        Serial.print("reboot in: ");
        Serial.print(MAX_NULL_READINGS_SEC - (uptime - lastNotNullReadings));
        Serial.println(" sec");

        if (lastNotNullReadings != 0 && uptime - lastNotNullReadings > MAX_NULL_READINGS_SEC)
        {
            ESP.restart();
        }

        Telemetry::TelemteryData telemetryData = Telemetry::TelemteryData();

        float maxTemp = DAY_MAX_TEMP - DAY_TEMP_TOLERANCE_WARM;
        float minTemp = DAY_MAX_TEMP - DAY_TEMP_TOLERANCE_COLD;

        if (!isDay)
        {
            maxTemp = NIGHT_MAX_TEMP - NIGHT_TEMP_TOLERANCE_WARM;
            minTemp = NIGHT_MAX_TEMP - NIGHT_TEMP_TOLERANCE_COLD;
        }

        if (heaterPhase == cooling)
        {
            if (hotSide.t <= minTemp && hotCenter.t <= minTemp)
            {
                turnRelayOn();
                telemetryData.heater = true;
            }
            else
            {
                turnRelayOff();
                telemetryData.heater = false;
            }
        }
        else
        {
            if (hotSide.t >= maxTemp || hotCenter.t >= maxTemp)
            {
                turnRelayOff();
                telemetryData.heater = false;
            }
            else
            {
                turnRelayOn();
                telemetryData.heater = true;
            }
        }

        telemetryData.hotSide = hotSide;

        if (SENSORS_COUNT == 2)
        {
            telemetryData.hotCenter = hotSide;
            telemetryData.coldCenter = coldSide;
        }
        else if (SENSORS_COUNT == 4)
        {
            telemetryData.hotCenter = hotCenter;
            telemetryData.coldCenter = coldCenter;
        }

        telemetryData.coldSide = coldSide;
        telemetryData.heaterPhase = heaterPhase;

        telemetryData.climateConfig.dayMaxTemp = DAY_MAX_TEMP;
        telemetryData.climateConfig.nightMaxTemp = NIGHT_MAX_TEMP;
        telemetryData.climateConfig.dayTempToleranceWarm = DAY_TEMP_TOLERANCE_WARM;
        telemetryData.climateConfig.dayTempToleranceCold = DAY_TEMP_TOLERANCE_COLD;
        telemetryData.climateConfig.nightTempToleranceWarm = NIGHT_TEMP_TOLERANCE_WARM;
        telemetryData.climateConfig.nightTempToleranceCold = NIGHT_TEMP_TOLERANCE_COLD;

        telemetryData.fullfilled = true;

        return telemetryData;
    }
}
