#include "net.h"
#include "secrets.h"

namespace Net
{

    void connect()
    {

        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        delay(100);

        WiFi.setHostname("Terrarium Controller v2.0");

        int attempts = 0;

        WiFi.begin(WIFI_SSID, WIFI_PASS);

        while (!WiFi.isConnected())
        {
            attempts++;
            Display::renderConnectingToWifi(WIFI_SSID, attempts);
            delay(1 * 1000);
            Serial.println(WiFi.status());
            if (attempts >= 20)
            {
                attempts = 0;
                WiFi.begin(WIFI_SSID, WIFI_PASS);
            }
        }

        // you're connected now, so print out the data:
        Serial.print("You're connected to the network");
        Display::renderConnectedToWifi(WIFI_SSID);
    }
}