// ============== LIBS ==============
// WiFi
#include <ESP8266WiFi.h>
// EEPROM
#include <EEPROM.h>
// MQTT
#include <GyverTimer.h>
#include <PubSubClient.h>
// BME280
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
// WEB
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Updater.h>
// File System
#include <FS.h>
// Word Time
#include <ESP8266mDNS.h>
#include <NTPClient.h>
// ============== LIBS ==============


// ============ SETTINGS ============
#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE
#define DEBUG_LN(x) Serial.println(x)
#define DEBUG(x) Serial.print(x)
#else
#define DEBUG_LN(x)
#define DEBUG(x)
#endif
// -------------- MQTT --------------
#define MQTT_TIMEOUT_CONNECT         5000
#define MQTT_TIMEOUT_PUBLISH_DEFAULT 3000

// ------------- BME280 -------------
#define SEALEVELPRESSURE_HPA (1013.25)
#define HPA_TO_MMHG          0.7500638

//-------------- SERIAL -------------
#define SPEED_SERIAL 115200

// -------------- WiFi --------------
#define AP_SSID_DEFAULT "BME280"
#define AP_PASS_DEFAULT ""

// ------------- EEPROM -------------
// WiFi
#define LENGHT_WIFI_SSID 32
#define LENGHT_WIFI_PSSD 63
// AP
#define LENGHT_ADDRESS_AP_NAME 32
#define LENGHT_ADDRESS_AP_PSSD 63
#define LENGHT_ADDRESS_AP_USE  1
// MQTT
#define LENGHT_MQTT_SERVER 40
#define LENGHT_MQTT_PORT   6
#define LENGHT_MQTT_USER   20
#define LENGHT_MQTT_PASS   20
#define LENGHT_MQTT_AUTH   1
#define LENGHT_MQTT        1
// Topics
#define LENGHT_MQTT_TOPIC_TEMP     20
#define LENGHT_MQTT_TOPIC_HUM      20
#define LENGHT_MQTT_TOPIC_PRESS    20
#define LENGHT_MQTT_TOPIC_ALTITUDE 20

#define LENGHT_MQTT_USE_TOPIC_TEMP     1
#define LENGHT_MQTT_USE_TOPIC_HUM      1
#define LENGHT_MQTT_USE_TOPIC_PRESS    1
#define LENGHT_MQTT_USE_TOPIC_ALTITUDE 1
// Settings
#define LENGHT_MQTT_TIMEOUT_PUBLISH 7

// WiFi
#define START_ADDRESS_WIFI_SSID           0
#define START_ADDRESS_WIFI_PSSD           LENGHT_WIFI_SSID + 1
// AP
#define START_ADDRESS_AP_NAME             START_ADDRESS_WIFI_PSSD + LENGHT_WIFI_PSSD + 1
#define START_ADDRESS_AP_PSSD             START_ADDRESS_AP_NAME + LENGHT_ADDRESS_AP_NAME + 1
#define START_ADDRESS_AP_USE              START_ADDRESS_AP_PSSD + LENGHT_ADDRESS_AP_PSSD + 1
// MQTT
#define START_ADDRESS_MQTT_SERVER         START_ADDRESS_AP_USE + LENGHT_ADDRESS_AP_USE + 1
#define START_ADDRESS_MQTT_PORT           START_ADDRESS_MQTT_SERVER + LENGHT_MQTT_SERVER + 1
#define START_ADDRESS_MQTT_USER           START_ADDRESS_MQTT_PORT + LENGHT_MQTT_PORT + 1
#define START_ADDRESS_MQTT_PASS           START_ADDRESS_MQTT_USER + LENGHT_MQTT_USER + 1
#define START_ADDRESS_MQTT_AUTH           START_ADDRESS_MQTT_PASS + LENGHT_MQTT_PASS + 1
#define START_ADDRESS_MQTT                START_ADDRESS_MQTT_AUTH + LENGHT_MQTT_AUTH + 1
// Topics
#define START_ADDRESS_TOPIC_TEMP          START_ADDRESS_MQTT + LENGHT_MQTT + 1
#define START_ADDRESS_TOPIC_HUM           START_ADDRESS_TOPIC_TEMP + LENGHT_MQTT_TOPIC_TEMP + 1
#define START_ADDRESS_TOPIC_PRESS         START_ADDRESS_TOPIC_HUM + LENGHT_MQTT_TOPIC_HUM + 1
#define START_ADDRESS_TOPIC_ALTITUDE      START_ADDRESS_TOPIC_PRESS + LENGHT_MQTT_TOPIC_PRESS + 1

#define START_ADDRESS_USE_TOPIC_TEMP      START_ADDRESS_TOPIC_ALTITUDE + LENGHT_MQTT_TOPIC_ALTITUDE + 1
#define START_ADDRESS_USE_TOPIC_HUM       START_ADDRESS_USE_TOPIC_TEMP + LENGHT_MQTT_USE_TOPIC_TEMP + 1
#define START_ADDRESS_USE_TOPIC_PRESS     START_ADDRESS_USE_TOPIC_HUM + LENGHT_MQTT_USE_TOPIC_HUM + 1
#define START_ADDRESS_USE_TOPIC_ALTITUDE  START_ADDRESS_USE_TOPIC_PRESS + LENGHT_MQTT_USE_TOPIC_PRESS + 1
// Settings
#define START_ADDRESS_TIMEOUT_PUBLISH     START_ADDRESS_USE_TOPIC_ALTITUDE + LENGHT_MQTT_USE_TOPIC_ALTITUDE + 1

#define EEPROM_LENGTH                     START_ADDRESS_TIMEOUT_PUBLISH + LENGHT_MQTT_TIMEOUT_PUBLISH
// ============ SETTINGS ============

// VARIABLES
#include "a0_data.h"

void setup() {
  Serial.begin(SPEED_SERIAL);
  Serial.println();

  // Time
  timeClient.begin();
  timeClient.setTimeOffset(10800);

  // Time reading values
  DEBUG("Timer publish, set interval:60000, start");
  timerReader.setInterval(60000);
  timerReader.start();

  // Initialize SPIFFS
  DEBUG_LN("Initialize SPIFFS");
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  beginEEPROM();
  readConfig();
  WiFiStart();
}

void loop() {
  timeClient.update();

  if (timerReader.isReady()) {
    if (!bme.begin(0x76)) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!"); // Печать, об ошибки инициализации.
    }
    readValues();
  }

  if (!isAP && useMQTT) mqttLoop();
}
