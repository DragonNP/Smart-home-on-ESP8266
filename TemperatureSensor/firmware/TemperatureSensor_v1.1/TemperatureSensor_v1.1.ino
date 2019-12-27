/*
  Sketch for the project "Temperature sensor"
  The source code on GitHub: https://github.com/DragonNP/Smart-home-on-ESP8266/TemperatureSensor/
  Author: DragonNP, 2019
  https://github.com/DragonNP/
*/

/*
  Version 1.1.1
  - Created project
*/

// ============ SETTINGS ============
#define BLYNK_PRINT Serial        // Comment this out to disable prints and save space
#define INTERVAL_TIMER 1000       // Set the time (in milliseconds) to poll the temperature sensor
#define BLYNK_TOKEN "token"       // Blynk token

// -------- WiFi Manager ---------
#define AC_SSID "Temperature sensor"
#define AC_PASS ""

// ------------ PINS -------------
#define TEMP_SENSOR_INPUT_PIN 2   // Pin on which the temperature sensor is installed
#define TEMP_SENSOR_OUTPUT_PIN V1 // The temperature is sent to this pin
// =========== SETTINGS ===========


// ============= LIBS =============
#include <SPI.h>                  //https://github.com/esp8266/Arduino/tree/master/libraries/SPI
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <DNSServer.h>            //https://github.com/esp8266/Arduino/tree/master/libraries/DNSServer
#include <ESP8266WebServer.h>     //https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer
#include <BlynkSimpleEsp8266.h>   //https://github.com/blynkkk/blynk-library
#include <GyverTimer.h>           //https://github.com/AlexGyver/GyverLibs/tree/master/GyverTimer
// ============= LIBS =============


char blynk_token[] = BLYNK_TOKEN;

GTimer tempTimer(MS);


void setup() {
  Serial.begin(115200);

  // Init WiFi Manager
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  WiFiManagerParameter custom_blynk_token("Blynk", "blynk token", blynk_token, 34);
  wifiManager.addParameter(&custom_blynk_token);
  wifiManager.autoConnect(AC_SSID, AC_PASS);

  Blynk.config(custom_blynk_token.getValue());

  tempTimer.setInterval(INTERVAL_TIMER);
  tempTimer.start();
}

void loop() {
  Blynk.run();
  if (tempTimer.isReady()) sendTemperature();
}

// Send Temperature in Blynk
void sendTemperature() {
  float temp = analogRead(TEMP_SENSOR_PIN);
  temp = temp * 0.48828125;
  Blynk.virtualWrite(CHART_PIN, temp);
}