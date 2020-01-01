/*
  Sketch for the project "Relay"
  The source code on GitHub: 
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
*/

// ============ SETTINGS ============
// -------- WiFi Manager ---------
#define AC_SSID "Relay"
#define AC_PASS ""

// ------------ PINS -------------
#define BUTTON_PIN V2   // Pin button for on/off relay
#define RELAY_PIN 2     // Pin relay

// --------- MQTT SERVER ---------
const char *mqtt_server = "server"; // Server name MQTT
const int mqtt_port = 1883;         // Port for connecting to the MQTT server
const char *mqtt_user = "login";    // Login from the server
const char *mqtt_pass = "passwd";   // The password from the server
#define TOPIK_OUT "/topik/out"
#define TOPIK_IN "/topik/in"
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
String stateRelay;

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  stateRelay = "ON";
  
  // Init WiFi Manager
  wifiManager.setDebugOutput(false);
  wifiManager.autoConnect(AC_SSID, AC_PASS);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && !client.connected()) {
      Serial.println("Connecting to MQTT server");
      
      if (client.connect(MQTT::Connect("arduinoClient2").set_auth(mqtt_user, mqtt_pass))) {
        Serial.println("Connected to MQTT server");
        client.set_callback(callback);
        client.subscribe(TOPIK_IN);
        client.publish(TOPIK_OUT, String(stateRelay));
      }else
        Serial.println("Could not connect to MQTT server"); 
  }
  
  if (client.connected())
    client.loop();
}

void callback(const MQTT::Publish& pub) {  
  if(String(pub.topic()) == TOPIK_IN) {
    stateRelay = pub.payload_string();
    Serial.println(pub.payload_string());
    
    if(stateRelay == "ON") 
      digitalWrite(RELAY_PIN, HIGH);
    else
      digitalWrite(RELAY_PIN, LOW);
      
    client.publish(TOPIK_OUT, stateRelay);
  }
}
