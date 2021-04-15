// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "main.h"

#define DHTPIN_1 18     // Digital pin connected to the DHT sensor 
#define DHTPIN_2 5     // Digital pin connected to the DHT sensor 
#define DHTPIN_3 17     // Digital pin connected to the DHT sensor 
#define DHTPIN_4 16     // Digital pin connected to the DHT sensor 

#define HALLPIN_1 25
#define HALLPIN_2 26

// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht1(DHTPIN_1, DHTTYPE);
DHT_Unified dht2(DHTPIN_2, DHTTYPE);
DHT_Unified dht3(DHTPIN_3, DHTTYPE);
DHT_Unified dht4(DHTPIN_4, DHTTYPE);


uint32_t delayMS = 1000;
int hallPin1Val;
int hallPin2Val;

void setup() {
  Serial.begin(9600);
  pinMode (HALLPIN_1, INPUT);
  pinMode (HALLPIN_2, INPUT);
  // Initialize device.
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
}

void loop() {
  // Delay between measurements.
  delay(delayMS);
  readTempHumid(dht1);
  readTempHumid(dht2);
  readTempHumid(dht3);
  readTempHumid(dht4);
  hallPin1Val = digitalRead(HALLPIN_1);
  hallPin2Val = digitalRead(HALLPIN_2);
  Serial.print("Hall sensors 1:");
  Serial.print(hallPin1Val);
  Serial.print(", 2:");
  Serial.print(hallPin2Val);
  Serial.println(F("------------------------------------"));
}

void readTempHumid(DHT_Unified dht){
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}