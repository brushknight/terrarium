#include "display.h"

namespace Display
{

int lcdColumns = 20;
int lcdRows = 4;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

    void renderClimate(Telemetry::TelemteryData telemteryData){
        lcd.clear(); 
        // set cursor to first column, first row
        lcd.setCursor(0, 0);
        // print message
        lcd.print("H.S ");
        lcd.setCursor(4, 0);
        // print message
        lcd.print(tempToString(telemteryData.hotSide.t));

        lcd.setCursor(10, 0);
        lcd.print("H.C ");
        lcd.setCursor(14, 0);
        lcd.print(tempToString(telemteryData.hotSide.t));

        lcd.setCursor(0, 1);
        lcd.print("C.C ");
        lcd.setCursor(4, 1);
        lcd.print(tempToString(telemteryData.hotSide.t));

        lcd.setCursor(10, 1);
        lcd.print("C.S ");
        lcd.setCursor(14, 1);
        lcd.print(tempToString(telemteryData.hotSide.t));

        lcd.setCursor(0, 2);
        lcd.print("heater ");
        lcd.setCursor(7, 2);
        if (telemteryData.heater){
            lcd.print("ON");
        }else{
            lcd.print("OFF");
        }
        lcd.setCursor(11, 2);
        if (telemteryData.heaterPhase == Climate::HeaterPhase::cooling){
            lcd.print("cooling");
        }else{
            lcd.print("heating");
        }
    }

    char * tempToString(double value){
        static char buffer [5];
        sprintf(buffer, "%.1fC", value);
        Serial.println(buffer);
        return buffer;
    }

    void renderConnectingToWifi(char* ssid){
        lcd.clear(); 
        // set cursor to first column, first row
        lcd.setCursor(0, 0);
        lcd.print("Connecting to WiFi");

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