/*
  Sketch for the project "Relay"
  The source code on GitHub:  https://github.com/DragonNP/Smart-home-on-ESP8266/tree/master/Relay
  Author: DragonNP, 2019
  https://github.com/DragonNP/
*/

/*
  Version 1.0
  - Created project

  Version 1.1
  - Minor update

  Version 2.0
  - Added management via OpenHab (MQTT Broker)

  Version 2.1
  - Small change
*/

// ============ SETTINGS ============
#define isDebug true

// -------- WiFi Manager ---------
#define AC_SSID "Relay"
#define AC_PASS ""

// ------------ PINS -------------
#define RELAY_PIN 2                 // Pin relay

// --------- MQTT SERVER ---------
const char *mqtt_server = "server"; // Server name MQTT
const int mqtt_port = 1883;         // Port for connecting to the MQTT server
const char *mqtt_user = "login";    // Login from the server
const char *mqtt_pass = "passwd";   // The password from the server
#define TOPIK "topik"
// =========== SETTINGS ===========


// ============= LIBS =============
#include <SPI.h>                  //https://github.com/esp8266/Arduino/tree/master/libraries/SPI
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <DNSServer.h>            //https://github.com/esp8266/Arduino/tree/master/libraries/DNSServer
#include <ESP8266WebServer.h>     //https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer
#include <PubSubClient.h>         //https://github.com/knolleary/pubsubclient
// ============= LIBS =============

WiFiManager wifiManager;
WiFiClient espClient;
PubSubClient client(espClient, mqtt_server, mqtt_port);
int stateRelay;

void setup() {
#if isDebug
  Serial.begin(115200);
#endif

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  stateRelay = HIGH;
  
  // Init WiFi Manager
#if !isDebug
  wifiManager.setDebugOutput(false);
#endif
  wifiManager.autoConnect(AC_SSID, AC_PASS);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && !client.connected()) {
#if isDebug
      Serial.println("Connecting to MQTT server");
#endif

      if (client.connect(MQTT::Connect(AC_SSID).set_auth(mqtt_user, mqtt_pass))) {
#if isDebug
        Serial.println("Connected to MQTT server");
#endif
        client.publish(TOPIK, String(stateRelay));
        client.set_callback(callback);
        client.subscribe(TOPIK);
      }
      else {
#if isDebug
        Serial.println("Could not connect to MQTT server"); 
#endif
      }
  }
  
  if (client.connected())
    client.loop();
}

void callback(const MQTT::Publish& pub) {
#if isDebug
  Serial.print(String(pub.topic()));
  Serial.print(" ==> ");
  Serial.println(pub.payload_string());
#endif
  
  stateRelay = pub.payload_string().toInt();
  digitalWrite(RELAY_PIN, stateRelay);
}
