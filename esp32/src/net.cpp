#include "net.h"
#include "secrets.h"

namespace Net
{

    void connect(){

        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        delay(100);

        WiFi.setHostname("Terr Controller");

        int status = WL_IDLE_STATUS;     // the Wifi radio's status

        // attempt to connect to Wifi network:
        while (status != WL_CONNECTED) {
            Serial.print("Attempting to connect SSID: ");
            Serial.println(WIFI_SSID);
            Display::renderConnectingToWifi(WIFI_SSID);
            status = WiFi.begin(WIFI_SSID, WIFI_PASS);

            // wait 10 seconds for connection:
            delay(10000);
        }

        // you're connected now, so print out the data:
        Serial.print("You're connected to the network");
        Display::renderConnectedToWifi(WIFI_SSID);

    }
}