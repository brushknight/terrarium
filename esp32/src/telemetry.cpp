#include "telemetry.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>

namespace Telemetry
{
    void send(TelemteryData telemteryData)
    {

        Net::connect(false);

        char telemetryEndpoint [200];
        sprintf (telemetryEndpoint, "http://terrarium.lab/api/v2/telemetry/%d", TERRARIUM_ID);
        Serial.print(telemetryEndpoint);

        //const char*  TELEMETRY_ENDPOINT = "http://terrarium.lab/api/v1/telemetry";

        HTTPClient http;

        http.begin(telemetryEndpoint);
        http.addHeader("Content-Type", "application/json");

        StaticJsonDocument<400> doc;

        doc["heater"] = telemteryData.heater;
        if (telemteryData.heaterPhase == cooling){
            doc["heater_phase"] = "cooling";
        }else{
            doc["heater_phase"] = "heating";
        }

        doc.createNestedObject("hot_side");
        doc["hot_side"]["H"] = telemteryData.hotSide.h;
        doc["hot_side"]["T"] = telemteryData.hotSide.t;


        doc.createNestedObject("hot_center");
        doc["hot_center"]["H"] = telemteryData.hotCenter.h;
        doc["hot_center"]["T"] = telemteryData.hotCenter.t;


        doc.createNestedObject("cold_center");
        doc["cold_center"]["H"] = telemteryData.coldCenter.h;
        doc["cold_center"]["T"] = telemteryData.coldCenter.t;


        doc.createNestedObject("cold_side");
        doc["cold_side"]["H"] = telemteryData.coldSide.h;
        doc["cold_side"]["T"] = telemteryData.coldSide.t;
       

        doc.createNestedObject("climate_config");
        doc["climate_config"]["day_max_temp"] = telemteryData.climateConfig.dayMaxTemp;
        doc["climate_config"]["night_max_temp"] = telemteryData.climateConfig.nightMaxTemp;
        doc["climate_config"]["day_temp_tolerance_warm"] = telemteryData.climateConfig.dayTempToleranceWarm;
        doc["climate_config"]["night_temp_tolerance_warm"] = telemteryData.climateConfig.nightTempToleranceWarm;
        doc["climate_config"]["day_temp_tolerance_cold"] = telemteryData.climateConfig.dayTempToleranceCold;
        doc["climate_config"]["night_temp_tolerance_cold"] = telemteryData.climateConfig.nightTempToleranceCold;


        String requestBody;
        serializeJson(doc, requestBody);

        //Serial.println(requestBody);

        int httpResponseCode = http.POST(requestBody);

        if (httpResponseCode > 0)
        {

            String response = http.getString();

            // Serial.println(httpResponseCode);
            // Serial.println(response);
            return;
        }
        else
        {

            Serial.printf("Error occurred while sending HTTP POST");
            return;
        }
    }
}