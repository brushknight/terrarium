#include "main.h"

uint32_t lastTimeReinit = 0;
Display::DisplayData displayData = Display::DisplayData();
Telemetry::TelemteryData gTelemteryData = Telemetry::TelemteryData();

static portMUX_TYPE display_mutex;

#define HARVESTING_INTERVAL_SEC 5
#define SUBMISSION_INTERVAL_SEC 30
#define DISPLAY_REFRESH_INTERVAL 1
#define DISPLAY_REINIT_INTERVAL 60 * 3
#define SAMPLE_DUPLICATIONS_COUNT_ALLOWED 3

// Set web server port number to 80
WiFiServer server(80);
// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void submitTelemetry(void *parameter)
{

  int lastSampleIdSent = 0;
  int sampleDuplicationSinceLastSent = 0;

  for (;;)
  {
    if (lastSampleIdSent != gTelemteryData.sampleId && lastSampleIdSent < gTelemteryData.sampleId)
    {
      Serial.println("submission started");
      displayData.submission = true;
      Telemetry::send(gTelemteryData);
      lastSampleIdSent = gTelemteryData.sampleId;
      displayData.submission = false;
      Serial.print("submission finished, sample ID: ");
      Serial.println(lastSampleIdSent);
    }
    else
    {
      Serial.println("sample already sent");
      gTelemteryData = Telemetry::TelemteryData();
      sampleDuplicationSinceLastSent++;
    }

    if (sampleDuplicationSinceLastSent > SAMPLE_DUPLICATIONS_COUNT_ALLOWED)
    {
      ESP.restart();
    }

    // Pause before next submission interval
    vTaskDelay(1000 * SUBMISSION_INTERVAL_SEC / portTICK_PERIOD_MS);
  }
}

void statusLed(void *parameter)
{
  for (;;)
  {
    Status::connectingToWifi();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Status::startHttpServer();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Status::fetchTime();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Status::ok();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Status::warning();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Status::error();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void climateControl(void *parameter)
{

  int uptime = RealTime::getUptime();

  for (;;)
  {
    uptime = RealTime::getUptime();
    Telemetry::TelemteryData telemteryData = Climate::control(RealTime::getHour(), RealTime::getMinute(), uptime);
    telemteryData.second = RealTime::getSecond();
    telemteryData.version = VERSION;

    gTelemteryData = telemteryData;

    displayData.hotSide = telemteryData.hotSide;
    displayData.hotCenter = telemteryData.hotCenter;
    displayData.coldCenter = telemteryData.coldCenter;
    displayData.coldSide = telemteryData.coldSide;
    displayData.hotZoneHeater = telemteryData.hotZoneHeater;
    displayData.coldZoneHeater = telemteryData.coldZoneHeater;
    displayData.hotZoneHeaterPhase = telemteryData.hotZoneHeaterPhase;
    displayData.coldZoneHeaterPhase = telemteryData.coldZoneHeaterPhase;

    vTaskDelay(HARVESTING_INTERVAL_SEC * 1000 / portTICK_PERIOD_MS);
  }
}

void renderDisplay(void *parameter)
{
  for (;;)
  {
    Serial.println("renderDisplay");
    if (DISPLAY_ENABLED)
    {
      displayData.nextHarvestInSec = 0;

      displayData.terrId = TERRARIUM_ID;
      displayData.hour = RealTime::getHour();
      displayData.minute = RealTime::getMinute();
      displayData.second = RealTime::getSecond();
      //portENTER_CRITICAL(&display_mutex);
      Display::render(displayData);
      //portEXIT_CRITICAL(&display_mutex);
    }

    vTaskDelay(DISPLAY_REFRESH_INTERVAL * 1000 / portTICK_PERIOD_MS);
  }
}

void startHttpServer(void *parameter)
{
  for (;;)
  {
    WiFiClient client = server.available(); // Listen for incoming clients

    if (!client)
    {
      vTaskDelay(100 * SUBMISSION_INTERVAL_SEC / portTICK_PERIOD_MS);
      continue;
    }

    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    { // loop while the client's connected
      currentTime = millis();
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off

            // Display the HTML web page
            client.print("wifi_network ");
            client.print(WIFI_SSID);
            client.print("\n");

            // temperature
            client.print("temperature_cold_side ");
            client.print(gTelemteryData.coldSide.t);
            client.print("\n");

            client.print("temperature_cold_center ");
            client.print(gTelemteryData.coldCenter.t);
            client.print("\n");

            client.print("temperature_hot_center ");
            client.print(gTelemteryData.hotCenter.t);
            client.print("\n");

            client.print("temperature_hot_side ");
            client.print(gTelemteryData.hotSide.t);
            client.print("\n");

            // humidity
            client.print("humidity_cold_side ");
            client.print(gTelemteryData.coldSide.h);
            client.print("\n");

            client.print("humidity_cold_center ");
            client.print(gTelemteryData.coldCenter.h);
            client.print("\n");

            client.print("humidity_hot_center ");
            client.print(gTelemteryData.hotCenter.h);
            client.print("\n");

            client.print("humidity_hot_side ");
            client.print(gTelemteryData.hotSide.h);
            client.print("\n");

            // heater status
            client.print("hot_zone_heater ");
            client.print(gTelemteryData.hotZoneHeater);
            client.print("\n");

            client.print("cold_zone_heater ");
            client.print(gTelemteryData.coldZoneHeater);
            client.print("\n");

            client.print("hot_zone_heater_phase ");
            client.print(gTelemteryData.hotZoneHeaterPhase);
            client.print("\n");

            client.print("cold_zone_heater_phase ");
            client.print(gTelemteryData.coldZoneHeaterPhase);
            client.print("\n");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void setup()
{
  Serial.begin(115200);

  vPortCPUInitializeMutex(&display_mutex);

  if (DISPLAY_ENABLED)
  {
    Display::displaySetup();
    Display::bootScreen();
  }

  if (RTC_ENABLED)
  {
    RealTime::setupRtcModule();
  }
  else
  {
    RealTime::setupWithoutRTC();
  }

  int uptime = RealTime::getUptime();
  Climate::setup(uptime);

  xTaskCreatePinnedToCore(
      submitTelemetry,
      "submitTelemetry",
      1024 * 10,
      NULL,
      1,
      NULL,
      0);

  Status::setup();

  xTaskCreatePinnedToCore(
      statusLed,
      "statusLed",
      1024,
      NULL,
      2,
      NULL,
      1);

  xTaskCreatePinnedToCore(
      renderDisplay,
      "renderDisplay",
      1024 * 10,
      NULL,
      2,
      NULL,
      1);

  xTaskCreatePinnedToCore(
      climateControl,
      "climateControl",
      1024 * 10,
      NULL,
      3,
      NULL,
      0);

  server.begin();

  xTaskCreatePinnedToCore(
      startHttpServer,
      "startHttpServer",
      1024 * 10,
      NULL,
      3,
      NULL,
      1);
}

void loop()
{
  int uptime = RealTime::getUptime();

  if (uptime - lastTimeReinit >= DISPLAY_REINIT_INTERVAL)
  {
    Display::displaySetup();
    lastTimeReinit = uptime;
  }
}
