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
#define SENSORS_ENABLE_PIN 32
#define HOT_ZONE_HEATER_RELAY_PIN 4
#define COLD_ZONE_HEATER_RELAY_PIN 25
#define LAMP_RELAY_PIN 5

#define DHT_HOT_SIDE_PIN int(16)    // #1
#define DHT_HOT_CENTER_PIN int(17)  // #2
#define DHT_COLD_CENTER_PIN int(18) // #3
#define DHT_COLD_SIDE_PIN int(19)   // #4

#define DAY_TEMP_TOLERANCE_WARM 0
#define DAY_TEMP_TOLERANCE_COLD 0.2

#define MAX_NULL_READINGS_SEC 30

    DHTStable DHT;

    class DHT22CustomSensor
    {
    public:
        int pin;
        ClimateData current;
        ClimateData previous;
        // just for statistics
        int errorsInARow = 0;
        int succesfulReadingsCountInARow = 0;

        DHT22CustomSensor()
        {
        }

        DHT22CustomSensor(int p)
        {
            pin = p;
        }

        bool success()
        {
            return current.status == DHTLIB_OK;
        }

        bool isSet()
        {
            return pin > 0;
        }

        ClimateData getCurrent()
        {

            if (!isSet())
            {
                return ClimateData(0, 0, DHTLIB_ERROR_TIMEOUT);
            }

            return current;
        }

        bool read()
        {
            Serial.printf("[pin:%d] reading sensor\n", pin);

            Status::setClimateStatus(Status::WORKING);

            if (pin == 0)
            {
                Serial.printf("[pin:%d] sensor disabled\n", pin);
                return false;
            }

            int status = DHT.read22(pin);

            switch (status)
            {
            case DHTLIB_OK:
                previous = current;

                current.h = DHT.getHumidity();
                current.t = DHT.getTemperature();

                // TODO check for data error (more than X% in x Seconds)

                Serial.printf("[pin:%d] Temperature: %.2f°C\n", pin, current.t);
                Serial.printf("[pin:%d] Humidity: %.2f%%\n", pin, current.h);
                current.status = status;
                Status::setClimateStatus(Status::SUCCESS);
                return true;
            case DHTLIB_ERROR_CHECKSUM:
                //counter.crc_error++;
                Serial.printf("[pin:%d] Checksum error,\n", pin);
                current.h = 0;
                current.t = 0;
                current.status = status;
                Status::setClimateStatus(Status::WARNING);
                return false;
            case DHTLIB_ERROR_TIMEOUT:
                //counter.time_out++;
                Serial.printf("[pin:%d] Time out error,\n", pin);
                current.h = 0;
                current.t = 0;
                current.status = status;
                Status::setClimateStatus(Status::WARNING);
                return false;
            default:
                //counter.unknown++;
                Serial.printf("[pin:%d] Unknown error,\n", pin);
                current.h = 0;
                current.t = 0;
                current.status = status;
                Status::setClimateStatus(Status::WARNING);
                return false;
            }

            return false;
        }
    };

    class ClimateZone
    {
    public:
        std::string name;
        HeaterPhase heaterPhase;
        bool heaterStatus;
        int relayPin;
        DHT22CustomSensor sensor1;
        DHT22CustomSensor sensor2;
        int sensor1Pin;
        int sensor2Pin;
        bool isDay;
        float dayMaxTemp;
        float dayMinTemp;
        float nightMaxTemp;
        float nightMinTemp;
        // some sensors statistics
        int successReadsInARow;
        int errTimeoutsInARow;
        int errTimeoutCount;
        int successReadsCount;
        float successErrRatio;

        ClimateZone() {}

        ClimateZone(bool d, std::string n, int sensor1Pin, int sensor2Pin, int rp, float dTargetTemp, float nTargetTemp)
        {
            isDay = d;
            name = n;
            relayPin = rp;
            sensor1 = DHT22CustomSensor(sensor1Pin);
            sensor2 = DHT22CustomSensor(sensor2Pin);
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

            //ClimateData sensor1 = readSensor(sensor1Pin);
            //ClimateData sensor2 = readSensor(sensor2Pin);

            if (sensor1.isSet())
            {
                int retryAttempt = 0;
                while (!sensor1.read() && retryAttempt < MAX_SENSOR_RETRY)
                {
                    rebootSensorsWithWait();
                    retryAttempt++;
                }

                // handle error correction

                // if(!sensor1.success()){
                //     ESP.restart();
                // }
            }
            if (sensor2.isSet())
            {
                int retryAttempt = 0;
                while (!sensor2.read() && retryAttempt < MAX_SENSOR_RETRY)
                {
                    rebootSensorsWithWait();
                    retryAttempt++;
                }

                // handle error correction

                // if(!sensor1.success()){
                //     ESP.restart();
                // }
            }

            // // handle error data
            // if (sensor1.current.t == 0)
            // {
            //     sensor1 = sensor2;
            // }
            // else if (sensor2.current.t == 0)
            // {
            //     sensor2 = sensor1;
            // }

            // if (sensor1.current.t == 0 && sensor2.current.t == 0)
            // {
            //     // dont change anything if data is not harvested
            //     return;
            // }

            float tAvg = (sensor1.current.t + sensor2.current.t) / 2;

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
            if (sensor1.current.t < criticalMinTemp || sensor2.current.t < criticalMinTemp)
            {
                turnRelayOn();
                heaterStatus = true;
            }

            // too hot
            if (sensor1.current.t > criticalMaxTemp || sensor2.current.t > criticalMaxTemp)
            {
                turnRelayOff();
                heaterStatus = false;
            }
        }
    };

    int lastNotNullReadings = 0;
    int sampleId = 0;

    float hotZoneDayTargetTemnp = 28.5;
    float hotZoneNightTargetTemnp = 23;

    float coldZoneDayNightTargetTemnp = 23;

    ClimateZone hotZone;
    ClimateZone coldZone;

    void setup(uint32_t uptime)
    {
        pinMode(HOT_ZONE_HEATER_RELAY_PIN, OUTPUT);
        pinMode(COLD_ZONE_HEATER_RELAY_PIN, OUTPUT);
        pinMode(LAMP_RELAY_PIN, OUTPUT);
        pinMode(SENSORS_ENABLE_PIN, OUTPUT);
        digitalWrite(LAMP_RELAY_PIN, LOW);

        if (SENSORS_COUNT == 2)
        {
            hotZone = ClimateZone(
                false, "hot zone",
                DHT_HOT_SIDE_PIN,
                0,
                HOT_ZONE_HEATER_RELAY_PIN,

                hotZoneDayTargetTemnp,
                hotZoneNightTargetTemnp);

            coldZone = ClimateZone(
                false, "cold zone",
                DHT_COLD_SIDE_PIN,
                0,
                COLD_ZONE_HEATER_RELAY_PIN,

                coldZoneDayNightTargetTemnp,
                coldZoneDayNightTargetTemnp);
        }
        else if (SENSORS_COUNT == 4)
        {
            if (SENSORS_COUNT == 2)
            {
                hotZone = ClimateZone(
                    false, "hot zone",

                    DHT_HOT_SIDE_PIN,
                    DHT_HOT_CENTER_PIN,
                    HOT_ZONE_HEATER_RELAY_PIN,
                    hotZoneDayTargetTemnp,
                    hotZoneNightTargetTemnp);

                coldZone = ClimateZone(
                    false, "cold zone",

                    DHT_COLD_SIDE_PIN,
                    DHT_COLD_CENTER_PIN,
                    COLD_ZONE_HEATER_RELAY_PIN,
                    coldZoneDayNightTargetTemnp,
                    coldZoneDayNightTargetTemnp);
            }
        }

        lastNotNullReadings = uptime;
    }

    void rebootSensorsWithWait()
    {
        Status::setClimateStatus(Status::WARNING);
        disableSensors();
        delay(100);
        enableSensors();
        delay(3000); // wait for sensros to boot
        Status::setClimateStatus(Status::IDLE);
    }

    void enableSensors()
    {
        digitalWrite(SENSORS_ENABLE_PIN, HIGH);
    }

    void disableSensors()
    {
        digitalWrite(SENSORS_ENABLE_PIN, LOW);
    }

    int readThermistor(int pin)
    {
        THERMISTOR thermistor(
            pin,    // Analog pin
            10000,  // Nominal resistance at 25 ºC
            3950,   // thermistor's beta coefficient
            10000); // Value of the series resistor
        return thermistor.read();
    }

    Telemetry::TelemteryData control(int hour, int minute, uint32_t uptime)
    {

        Status::setClimateStatus(Status::WORKING);

        bool isDayNow = isDay(hour, minute);
        bool isWarning = false;

        hotZone.isDay = isDayNow;
        coldZone.isDay = isDayNow;

        hotZone.adjust();
        coldZone.adjust();

        // // rewrite this to move into climateZone
        // if ((hotZone.sensor1.current.t > 0 || hotZone.sensor2.current.t > 0) && (coldZone.sensor1.current.t > 0 || coldZone.sensor2.current.t > 0))
        // {
        //     lastNotNullReadings = uptime;
        //     isWarning = false;
        // }
        // else
        // {
        //     Status::setClimateStatus(Status::WARNING);
        //     isWarning = true;
        // }

        // if (lastNotNullReadings != 0 && uptime - lastNotNullReadings > MAX_NULL_READINGS_SEC)
        // {
        //     ESP.restart();
        // }

        if (DEBUG)
        {
            Serial.print("reboot in: ");
            Serial.print(MAX_NULL_READINGS_SEC - (uptime - lastNotNullReadings));
            Serial.println(" sec");
        }

        Telemetry::TelemteryData telemetryData = Telemetry::TelemteryData();

        telemetryData.hotCenter = hotZone.sensor2.getCurrent();
        telemetryData.hotSide = hotZone.sensor1.getCurrent();
        telemetryData.coldSide = coldZone.sensor1.getCurrent();
        telemetryData.coldCenter = coldZone.sensor2.getCurrent();

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

        // telemetryData.isDay = isDayNow;

        telemetryData.fullfilled = true;
        telemetryData.hour = hour;
        telemetryData.minute = minute;
        telemetryData.uptime = uptime;

        sampleId++;
        telemetryData.sampleId = sampleId;

        if (!isWarning)
        {
            Status::setClimateStatus(Status::IDLE);
        }

        return telemetryData;
    }

    bool isDay(int hour, int minute)
    {
        return hour >= DAY_START_HOUR && hour < NIGHT_START_HOUR && minute >= DAY_START_MINUTE;
    }
}
