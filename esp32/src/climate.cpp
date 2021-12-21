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
#define HOT_ZONE_HEATER_RELAY_PIN 4
#define COLD_ZONE_HEATER_RELAY_PIN 5

#define DHT_HOT_SIDE_PIN 16    // #1
#define DHT_HOT_CENTER_PIN 17  // #2
#define DHT_COLD_CENTER_PIN 18 // #3
#define DHT_COLD_SIDE_PIN 19   // #4

#define DAY_TEMP_TOLERANCE_WARM 0
#define DAY_TEMP_TOLERANCE_COLD 0.2

#define MAX_NULL_READINGS_SEC 30

    class ClimateZone
    {
    public:
        std::string name;
        HeaterPhase heaterPhase;
        bool heaterStatus;
        int relayPin;
        ClimateData sensor1;
        ClimateData sensor2;
        bool isDay;
        float dayMaxTemp;
        float dayMinTemp;
        float nightMaxTemp;
        float nightMinTemp;

        ClimateZone(bool d, std::string n, int rp, float dTargetTemp, float nTargetTemp)
        {
            isDay = d;
            name = n;
            relayPin = rp;
            dayMaxTemp = dTargetTemp - DAY_TEMP_TOLERANCE_WARM;
            dayMinTemp = dTargetTemp - DAY_TEMP_TOLERANCE_COLD;
            nightMaxTemp = nTargetTemp - DAY_TEMP_TOLERANCE_WARM;
            nightMinTemp = nTargetTemp - DAY_TEMP_TOLERANCE_COLD;
            heaterPhase = cooling;
        }

        void turnRelayOn()
        {
            heaterPhase = heating;
            digitalWrite(relayPin, HIGH);
        }

        void turnRelayOff()
        {
            heaterPhase = cooling;
            digitalWrite(relayPin, LOW);
        }

        void adjust()
        {
            float maxTemp = dayMaxTemp;
            float minTemp = dayMinTemp;

            if (!isDay)
            {
                maxTemp = nightMaxTemp;
                minTemp = nightMinTemp;
            }

            float criticalMinTemp = minTemp - 1;
            float criticalMaxTemp = maxTemp + 1;

            // handle error data
            if (sensor1.t == 0)
            {
                sensor1 = sensor2;
            }
            else if (sensor2.t == 0)
            {
                sensor2 = sensor1;
            }

            if (sensor1.t == 0 && sensor2.t == 0)
            {
                // dont change anything if data is not harvested
                return;
            }

            float tAvg = (sensor1.t + sensor2.t) / 2;

            if (heaterPhase == cooling)
            {

                if (tAvg <= minTemp)
                {
                    turnRelayOn();
                    heaterStatus = true;
                }
                else
                {
                    turnRelayOff();
                    heaterStatus = false;
                }
            }
            else
            {
                // heating
                if (tAvg >= maxTemp)
                {
                    turnRelayOff();
                    heaterStatus = false;
                }
                else
                {
                    turnRelayOn();
                    heaterStatus = true;
                }
            }

            // edge cases
            // too cold
            if (sensor1.t < criticalMinTemp || sensor2.t < criticalMinTemp)
            {
                turnRelayOn();
                heaterStatus = true;
            }

            // too hot
            if (sensor1.t > criticalMaxTemp || sensor2.t > criticalMaxTemp)
            {
                turnRelayOff();
                heaterStatus = false;
            }
        }
    };

    int lastNotNullReadings = 0;
    int sampleId = 0;

    DHT_Unified dhtHotSide(DHT_HOT_SIDE_PIN, DHTTYPE);
    DHT_Unified dhtHotCenter(DHT_HOT_CENTER_PIN, DHTTYPE);
    DHT_Unified dhtColdCenter(DHT_COLD_CENTER_PIN, DHTTYPE);
    DHT_Unified dhtColdSide(DHT_COLD_SIDE_PIN, DHTTYPE);

    ClimateData readTempHumid(DHT_Unified dht)
    {
        ClimateData data;

        sensors_event_t event;
        dht.temperature().getEvent(&event);
        if (isnan(event.temperature))
        {
            Serial.println(F("Error reading temperature!"));
            data.t = 0;
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
            data.h = 0;
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

    float hotZoneDayTargetTemnp = 28.5;
    float hotZoneNightTargetTemnp = 23;

    float coldZoneDayNightTargetTemnp = 23;

    ClimateZone hotZone = ClimateZone(
        false, "hot zone", HOT_ZONE_HEATER_RELAY_PIN,
        hotZoneDayTargetTemnp,
        hotZoneNightTargetTemnp);

    ClimateZone coldZone = ClimateZone(
        false, "cold zone", COLD_ZONE_HEATER_RELAY_PIN,
        coldZoneDayNightTargetTemnp,
        coldZoneDayNightTargetTemnp);

    void setup(uint32_t uptime)
    {
        pinMode(HOT_ZONE_HEATER_RELAY_PIN, OUTPUT);
        pinMode(COLD_ZONE_HEATER_RELAY_PIN, OUTPUT);

        dhtHotSide.begin();
        dhtHotCenter.begin();
        dhtColdCenter.begin();
        dhtColdSide.begin();

        lastNotNullReadings = uptime;
    }

    Telemetry::TelemteryData control(int hour, int minute, uint32_t uptime)
    {

        bool isDay = hour >= DAY_START_HOUR && hour < NIGHT_START_HOUR && minute >= DAY_START_MINUTE;

        hotZone.isDay = isDay;
        hotZone.sensor1 = readTempHumid(dhtHotSide);
        hotZone.sensor2 = readTempHumid(dhtHotCenter);

        coldZone.isDay = isDay;
        coldZone.sensor1 = readTempHumid(dhtColdCenter);
        coldZone.sensor2 = readTempHumid(dhtColdSide);

        if ((hotZone.sensor1.t > 0 || hotZone.sensor2.t > 0) && (coldZone.sensor1.t > 0 || coldZone.sensor2.t > 0))
        {
            lastNotNullReadings = uptime;
        }

        if (lastNotNullReadings != 0 && uptime - lastNotNullReadings > MAX_NULL_READINGS_SEC)
        {
            ESP.restart();
        }

        hotZone.adjust();
        coldZone.adjust();

        if (DEBUG)
        {
            Serial.print("reboot in: ");
            Serial.print(MAX_NULL_READINGS_SEC - (uptime - lastNotNullReadings));
            Serial.println(" sec");
        }

        Telemetry::TelemteryData telemetryData = Telemetry::TelemteryData();

        telemetryData.hotSide = hotZone.sensor1;
        telemetryData.hotCenter = hotZone.sensor2;
        telemetryData.coldCenter = coldZone.sensor1;
        telemetryData.coldSide = coldZone.sensor2;
        telemetryData.hotZoneHeaterPhase = hotZone.heaterPhase;
        telemetryData.coldZoneHeaterPhase = coldZone.heaterPhase;
        telemetryData.hotZoneHeater = hotZone.heaterStatus;
        telemetryData.coldZoneHeater = coldZone.heaterStatus;

        // telemetryData.climateConfig.dayMaxTemp = DAY_MAX_TEMP;
        // telemetryData.climateConfig.nightMaxTemp = NIGHT_MAX_TEMP;
        // telemetryData.climateConfig.dayTempToleranceWarm = DAY_TEMP_TOLERANCE_WARM;
        // telemetryData.climateConfig.dayTempToleranceCold = DAY_TEMP_TOLERANCE_COLD;
        // telemetryData.climateConfig.nightTempToleranceWarm = NIGHT_TEMP_TOLERANCE_WARM;
        // telemetryData.climateConfig.nightTempToleranceCold = NIGHT_TEMP_TOLERANCE_COLD;

        // telemetryData.isDay = isDay;

        telemetryData.fullfilled = true;
        telemetryData.hour = hour;
        telemetryData.minute = minute;
        telemetryData.uptime = uptime;

        sampleId++;
        telemetryData.sampleId = sampleId;

        return telemetryData;
    }
}
