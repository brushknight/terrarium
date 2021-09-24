#include "net.h"
#include "secrets.h"

namespace Net
{

    void connect(bool interactive)
    {

        if (WiFi.isConnected()){
            return;
        }

        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        delay(100);

        

        char buffer[100];
        sprintf(buffer, "%s#%d", "Terrarium controller ID", TERRARIUM_ID);

        WiFi.setHostname(buffer);

        int attempts = 0;

        //WiFi.config(IPAddress(167772400), IPAddress(167772161), IPAddress(167772160), IPAddress(167772161),IPAddress(167772161));

        WiFi.begin(WIFI_SSID, WIFI_PASS);

        while (!WiFi.isConnected())
        {
            attempts++;
            if (interactive){
            Display::renderConnectingToWifi(WIFI_SSID, attempts);
            }
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
        if (interactive){
        Display::renderConnectedToWifi(WIFI_SSID);
        }
    }
}