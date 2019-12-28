/*
  Sketch for the project "Relay"
  The source code on GitHub: 
  Author: DragonNP, 2019
  https://github.com/DragonNP/
*/

/*
  Version 1.1
  - Created project
*/

// ============ SETTINGS ============
#define BLYNK_PRINT Serial        // Comment this out to disable prints and save space
#define BLYNK_TOKEN "token"       // Blynk token

// -------- WiFi Manager ---------
#define AC_SSID "Rele"
#define AC_PASS ""

// ------------ PINS -------------
#define BUTTON_PIN V2   // Pin button for on/off relay
#define RELAY_PIN 2     // Pin relay
// =========== SETTINGS ===========


// ============= LIBS =============
#include <SPI.h>                  //https://github.com/esp8266/Arduino/tree/master/libraries/SPI
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <DNSServer.h>            //https://github.com/esp8266/Arduino/tree/master/libraries/DNSServer
#include <ESP8266WebServer.h>     //https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer
#include <BlynkSimpleEsp8266.h>   //https://github.com/blynkkk/blynk-library
// ============= LIBS =============

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  
  // Init WiFi Manager
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  wifiManager.autoConnect(AC_SSID, AC_PASS);
  
  // Init Blynk
  Blynk.config(BLYNK_TOKEN);
}

void loop() {
  Blynk.run();
}

BLYNK_WRITE(BUTTON_PIN) {
  digitalWrite(RELAY_PIN, param.asInt());
}
