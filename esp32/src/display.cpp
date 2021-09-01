#include "display.h"

namespace Display
{

    int lcdColumns = 20;
    int lcdRows = 4;

    LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

    void renderClimate(Telemetry::TelemteryData telemteryData)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(tempToString(telemteryData.hotSide.t));
        lcd.print("|");
        lcd.setCursor(5, 0);
        lcd.print(tempToString(telemteryData.hotSide.t));
        lcd.setCursor(11, 0);
        lcd.print(tempToString(telemteryData.hotSide.t));
        lcd.print("|");
        lcd.setCursor(16, 0);
        lcd.print(tempToString(telemteryData.hotSide.t));

        lcd.setCursor(0, 2);
        lcd.print("heater");
        lcd.setCursor(0, 3);
        if (telemteryData.heater)
        {
            lcd.print("ON");
        }
        else
        {
            lcd.print("OFF");
        }
        lcd.setCursor(13, 2);
        if (telemteryData.heaterPhase == Climate::HeaterPhase::cooling)
        {
            lcd.print("cooling");
        }
        else
        {
            lcd.print("heating");
        }
    }

    char *tempToString(double value)
    {
        static char buffer[5];
        sprintf(buffer, "%.1f", value);
        //Serial.println(buffer);
        return buffer;
    }

    void renderTime(int hour, int minute, int second)
    {
        char buffer[20];
        sprintf(buffer, "%02d:%02d:%02d", hour, minute, second);

        lcd.setCursor(12, 3);
        lcd.print(buffer);
    }

    void renderConnectingToWifi(char *ssid, int attempts)
    {
        lcd.clear();
        // set cursor to first column, first row
        lcd.setCursor(0, 0);
        lcd.print("Connecting to WiFi");

        lcd.setCursor(0, 1);
        lcd.print(ssid);

        lcd.setCursor(0, 2);
        for (int i = 0; i < attempts; i++)
        {
            lcd.print("*");
        }
    }

    void renderNtp(int attempts){
        lcd.clear();
        // set cursor to first column, first row
        lcd.setCursor(0, 0);
        lcd.print("Fetching time from NTP");

        lcd.setCursor(0, 1);
        for (int i = 0; i < attempts; i++)
        {
            lcd.print("*");
        }
    }

    void renderConnectedToWifi(char *ssid)
    {
        lcd.clear();
        // set cursor to first column, first row
        lcd.setCursor(0, 0);
        lcd.print("Connected to WiFi");

        lcd.setCursor(0, 1);
        lcd.print(ssid);
    }

    void displaySetup()
    {
        // initialize LCD
        lcd.init();
        // turn on LCD backlight
        lcd.backlight();
        Serial.println("displaySetup: ok");
    }
}