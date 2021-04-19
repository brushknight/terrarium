#include "climate.h"

namespace Climate
{

    struct climateData
    {
        float t;
        float h;
    };

    /* schedule

08 - 20 hot max 29.5
20 - 8 hot max 25

// temp tolerance ~ 1.5C - to be measured and tuned
 

*/

#define DHTTYPE DHT22
#define HEATER_RELAY_PIN 4

#define DHT_HOT_SIDE_PIN 18    // #1
#define DHT_HOT_CENTER_PIN 5   // #2
#define DHT_COLD_CENTER_PIN 17 // #3
#define DHT_COLD_SIDE_PIN 16   // #4

#define DAY_MAX_TEMP 30
#define DAY_TEMP_TOLERANCE 1
#define NIGHT_MAX_TEMP 24
#define NIGHT_TEMP_TOLERANCE 1

    DHT_Unified dhtHotSide(DHT_HOT_SIDE_PIN, DHTTYPE);
    DHT_Unified dhtHotCenter(DHT_HOT_CENTER_PIN, DHTTYPE);
    DHT_Unified dhtColdCenter(DHT_COLD_CENTER_PIN, DHTTYPE);
    DHT_Unified dhtColdSide(DHT_COLD_SIDE_PIN, DHTTYPE);

    volatile byte relayState = LOW;

    climateData readTempHumid(DHT_Unified dht)
    {
        climateData data;

        // Get temperature event and print its value.
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
        relayState = HIGH;
        digitalWrite(HEATER_RELAY_PIN, relayState);
    }

    void turnRelayOff()
    {
        relayState = LOW;
        digitalWrite(HEATER_RELAY_PIN, relayState);
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

    void climateControl()
    {

        // read all temps

        // if hot 1,2 temp is > limit
        // turn heating off
        // if hot 1,2 temp is < limit - tolerance
        // turn heating on

        climateData hotSide = readTempHumid(dhtHotSide);
        climateData hotCenter = readTempHumid(dhtHotCenter);
        climateData coldCenter = readTempHumid(dhtColdCenter);
        climateData coldSide = readTempHumid(dhtColdSide);

        if (hotSide.t > DAY_MAX_TEMP || hotCenter.t > DAY_MAX_TEMP) {
            turnRelayOff();
        }else if (hotSide.t < DAY_MAX_TEMP - DAY_TEMP_TOLERANCE && hotCenter.t < DAY_MAX_TEMP - DAY_TEMP_TOLERANCE) {
            turnRelayOn();
        }

    }

}

// define temparature schedule

// function to read sensors data

// function to check senrsors temps and switch heating
