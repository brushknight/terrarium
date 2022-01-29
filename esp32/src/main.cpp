#include "main.h"

uint32_t lastTimeReinit = 0;
Display::DisplayData displayData = Display::DisplayData();
Telemetry::TelemteryData gTelemteryData = Telemetry::TelemteryData();

#define HARVESTING_INTERVAL_SEC 5
#define SUBMISSION_INTERVAL_SEC 30
#define DISPLAY_REFRESH_INTERVAL 1
#define DISPLAY_REINIT_INTERVAL 60 * 1
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
    Status::handleStatusLed();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void climateControl(void *parameter)
{

  int uptime = RealTime::getUptimeSec();

  for (;;)
  {
    uptime = RealTime::getUptimeSec();
    //portENTER_CRITICAL(&display_mutex);
    int hour = RealTime::getHour();
    int minute = RealTime::getMinute();
    //portEXIT_CRITICAL(&display_mutex);
    Telemetry::TelemteryData telemteryData = Climate::control(hour, minute, uptime);

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

  int uptime;

  for (;;)
  {
    if (DISPLAY_ENABLED)
    {

      uptime = RealTime::getUptimeSec();

      if (uptime - lastTimeReinit >= DISPLAY_REINIT_INTERVAL)
      {
        Display::displaySetup();
        lastTimeReinit = uptime;
      }

      displayData.nextHarvestInSec = 0;

      displayData.terrId = TERRARIUM_ID;
      displayData.hour = RealTime::getHour();
      displayData.minute = RealTime::getMinute();
      displayData.second = RealTime::getSecond();

      Serial.printf("%d: ready to render screen\n", uptime);
      Display::render(displayData);
      Serial.printf("%d ready display finished\n", uptime);
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

            // system info
            client.print("id ");
            client.print(TERRARIUM_ID);
            client.print("\n");

            client.print("version ");
            client.print(VERSION);
            client.print("\n");

            client.print("display ");
            client.print(DISPLAY_ENABLED);
            client.print("\n");

            client.print("rtc ");
            client.print(RTC_ENABLED);
            client.print("\n");

            client.print("sensors_count ");
            client.print(SENSORS_COUNT);
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

  if (STATUS_LED_ENABLED)
  {
    Status::setup();

    xTaskCreatePinnedToCore(
        statusLed,
        "statusLed",
        1024,
        NULL,
        1,
        NULL,
        1);
  }

  if (DISPLAY_ENABLED)
  {
    Display::displaySetup();
    Display::bootScreen();

    xTaskCreatePinnedToCore(
        renderDisplay,
        "renderDisplay",
        1024 * 20,
        NULL,
        2,
        NULL,
        1);
  }

  RealTime::setup(RTC_ENABLED);

  int uptime = RealTime::getUptimeSec();
  Climate::setup(uptime);
  Climate::enableSensors();

  xTaskCreatePinnedToCore(
      submitTelemetry,
      "submitTelemetry",
      1024 * 10,
      NULL,
      1,
      NULL,
      0);

  xTaskCreatePinnedToCore(
      climateControl,
      "climateControl",
      1024 * 10,
      NULL,
      3,
      NULL,
      0);

  if (SERVER_ENABLED)
  {
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

  if (TEST_BOARD)
  {

    Status::setup();
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(25, OUTPUT);
    pinMode(32, OUTPUT);

    time_t now;
    char strftime_buf[64];
    struct tm timeinfo;

    time(&now);
    // Set timezone to China Standard Time
    //setenv("TZ", "CST-8", 1);
    //tzset();

    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    Serial.printf("The current date/time is: %s", strftime_buf);

    RealTime::setup(RTC_ENABLED);

    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    Serial.printf("The current date/time is: %s", strftime_buf);

    if (DISPLAY_ENABLED)
    {
      Display::displaySetup();
      Display::bootScreen();
    }

    if (SENSORS_ENABLED)
    {
      Climate::setup(0);
    }

    return;
  }
}

void testDHT(int pin)
{
  DHTStable DHT;

  int status = DHT.read22(pin);

  float h = 0;
  float t = 0;

  switch (status)
  {
  case DHTLIB_OK:
    //counter.ok++;
    h = DHT.getHumidity();
    t = DHT.getTemperature();
    Serial.print("OK,\t");
    break;
  case DHTLIB_ERROR_CHECKSUM:
    //counter.crc_error++;
    Serial.print("Checksum error,\t");
    break;
  case DHTLIB_ERROR_TIMEOUT:
    //counter.time_out++;
    Serial.print("Time out error,\t");
    break;
  default:
    //counter.unknown++;
    Serial.print("Unknown error,\t");
    break;
  }

  Serial.printf("t %f h %f\n", t, h);
}

void testTHermistor(int pin)
{
  int temp = Climate::readThermistor(pin);

  Serial.printf("temp themistor on pin:%d %dC", pin, temp);
}

void loop()
{

  if (TEST_BOARD)
  {

    Climate::enableSensors();
    // delay to enable sensors
    delay(2000);

    testDHT(16);
    delay(500);
    testDHT(17);
    delay(500);
    testDHT(18);
    delay(500);
    testDHT(19);
    delay(500);
    testDHT(26);
    delay(500);
    testDHT(27);
    delay(500);
    Climate::disableSensors();
    //testTHermistor(26);
    // testDHT(17);
    // testDHT(18);
    // testDHT(19);
    // testDHT(26);
    // testDHT(27);

    Serial.print("battery: ");
    Serial.print(RealTime::getBatteryPercent());
    Serial.print("% ");

    Serial.print(RealTime::getBatteryVoltage());
    Serial.println("mV");

    Serial.println("on");
    //Status::ok();
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(25, HIGH);

    Serial.print(RealTime::getHour());
    Serial.print(":");
    Serial.print(RealTime::getMinute());
    Serial.print(":");
    Serial.println(RealTime::getSecond());
    delay(500);
    Status::setWarning();
    delay(500);
    Serial.println("off");
    Status::setError();
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(25, LOW);
    delay(500);
    Status::setPink();
    delay(500);
  }
  else
  {
    vTaskDelete(NULL);
  }
}
