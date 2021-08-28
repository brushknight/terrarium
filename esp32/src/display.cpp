#include "display.h"

namespace Display
{

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

    void renderClimate(Telemetry::TelemteryData telemteryData){
        lcd.clear(); 
        // set cursor to first column, first row
        lcd.setCursor(0, 0);
        // print message
        lcd.print("t1:");
        lcd.setCursor(4, 0);
        // print message

        char buffer [5];
        sprintf(buffer, "%.1fC", telemteryData.hotSide.t);

        lcd.print(buffer);
    }

    void renderConnectingToWifi(char* ssid){
        lcd.clear(); 
        // set cursor to first column, first row
        lcd.setCursor(0, 0);
        lcd.print("Connecting to WiFi...");

        lcd.setCursor(0, 1);
        lcd.print(ssid);

    }

    void renderConnectedToWifi(char* ssid){
        lcd.clear(); 
        // set cursor to first column, first row
        lcd.setCursor(0, 0);
        lcd.print("Connected to WiFi");

        lcd.setCursor(0, 1);
        lcd.print(ssid);
    }

    void displaySetup(){
        // initialize LCD
        lcd.init();
        // turn on LCD backlight                      
        lcd.backlight();
        Serial.println("displaySetup: ok");
    }
}