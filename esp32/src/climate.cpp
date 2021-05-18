#include "climate.h"

namespace Climate
{

    /* schedule

08 - 20 hot max 29.5
20 - 8 hot max 25

// temp tolerance ~ 1.5C - to be measured and tuned
 

*/

#define DAY_START_HOUR 8
#define DAY_START_MINUTE 0

#define NIGHT_START_HOUR 20
#define NIGHT_START_MINUTE 0

#define DHTTYPE DHT22
#define HEATER_RELAY_PIN 4

#define DHT_HOT_SIDE_PIN 16   // #1
#define DHT_HOT_CENTER_PIN 17 // #2
#define DHT_COLD_CENTER_PIN 5 // #3
#define DHT_COLD_SIDE_PIN 18  // #4

#define DAY_MAX_TEMP 30
#define DAY_TEMP_TOLERANCE_WARM 0.5
#define DAY_TEMP_TOLERANCE_COLD 0.1
#define NIGHT_MAX_TEMP 24
#define NIGHT_TEMP_TOLERANCE_WARM 0.5
#define NIGHT_TEMP_TOLERANCE_COLD 0.1

    DHT_Unified dhtHotSide(DHT_HOT_SIDE_PIN, DHTTYPE);
    DHT_Unified dhtHotCenter(DHT_HOT_CENTER_PIN, DHTTYPE);
    DHT_Unified dhtColdCenter(DHT_COLD_CENTER_PIN, DHTTYPE);
    DHT_Unified dhtColdSide(DHT_COLD_SIDE_PIN, DHTTYPE);

    volatile byte relayState = LOW;


    HeaterPhase heaterPhase;

    ClimateData readTempHumid(DHT_Unified dht)
    {
        ClimateData data;

        // Get temperature event and print its value.
        sensors_event_t event;
        dht.temperature().getEvent(&event);
        if (isnan(event.temperature))
        {
            // Serial.println(F("Error reading temperature!"));
        }
        else
        {
            data.t = event.temperature;
            // Serial.print(F("Temperature: "));
            // Serial.print(event.temperature);
            // Serial.println(F("°C"));
        }
        // Get humidity event and print its value.
        dht.humidity().getEvent(&event);
        if (isnan(event.relative_humidity))
        {
            // Serial.println(F("Error reading humidity!"));
        }
        else
        {
            data.h = event.relative_humidity;
            // Serial.print(F("Humidity: "));
            // Serial.print(event.relative_humidity);
            // Serial.println(F("%"));
        }
        return data;
    }

    void turnRelayOn()
    {
        relayState = LOW;
        heaterPhase = heating;
        digitalWrite(HEATER_RELAY_PIN, relayState);
        Serial.println("turn relay on");
    }

    void turnRelayOff()
    {
        relayState = HIGH;
        heaterPhase = cooling;
        digitalWrite(HEATER_RELAY_PIN, relayState);
        Serial.println("turn relay off");
    }

    void climateSetup()
    {
        pinMode(HEATER_RELAY_PIN, OUTPUT);
        turnRelayOff();
        dhtHotSide.begin();
        dhtHotCenter.begin();
        dhtColdCenter.begin();
        dhtColdSide.begin();
    }

    Telemetry::TelemteryData climateControl(int hour, int minute)
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
            if (hotSide.t < minTemp && hotCenter.t < minTemp)
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
            if (hotSide.t > maxTemp || hotCenter.t > maxTemp)
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
        telemetryData.hotCenter = hotCenter;
        telemetryData.coldCenter = coldCenter;
        telemetryData.coldSide = coldSide;
        telemetryData.heaterPhase = heaterPhase;

        telemetryData.climateConfig.dayMaxTemp = DAY_MAX_TEMP;
        telemetryData.climateConfig.nightMaxTemp = NIGHT_MAX_TEMP;
        telemetryData.climateConfig.dayTempToleranceWarm = DAY_TEMP_TOLERANCE_WARM;
        telemetryData.climateConfig.dayTempToleranceCold = DAY_TEMP_TOLERANCE_COLD;
        telemetryData.climateConfig.nightTempToleranceWarm = NIGHT_TEMP_TOLERANCE_WARM;
        telemetryData.climateConfig.nightTempToleranceCold = NIGHT_TEMP_TOLERANCE_COLD;

        return telemetryData;
    }
}
