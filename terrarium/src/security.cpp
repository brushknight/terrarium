#include "security.h"

namespace Security
{

#define HALLPIN_1 25
#define HALLPIN_2 26

    int hallPin1Val;
    int hallPin2Val;

    void securitySetup()
    {
        pinMode(HALLPIN_1, INPUT);
        pinMode(HALLPIN_2, INPUT);
    }

    void securityCheck()
    {
        hallPin1Val = digitalRead(HALLPIN_1);
        hallPin2Val = digitalRead(HALLPIN_2);
        Serial.print("Hall sensors 1:");
        Serial.print(hallPin1Val);
        Serial.print(", 2:");
        Serial.print(hallPin2Val);
        Serial.println(F("------------------------------------"));
    }

}
// define hall sensors
// define button to arm/disarm
// define led to indicate alarm status (use led strip with pattern?)

// function to check sensors state, arm state and trigger alarm if needed

// button to arm/disarm handling