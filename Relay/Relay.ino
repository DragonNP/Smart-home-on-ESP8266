/*
  Sketch for the project "Relay"
  The source code on GitHub:  https://github.com/DragonNP/Smart-home-on-ESP8266/tree/master/Relay
  Author: DragonNP, 2020
  https://github.com/DragonNP/

  Libs link:
  ------------------------------------------------------
  ESP8266WiFi     -    https://github.com/esp8266/Arduino
  WiFiManager     -    https://github.com/tzapu/WiFiManager
  PubSubClient    -    https://github.com/ianscrivener/pubsubclient
  Adafruit_Sensor -    https://github.com/adafruit/Adafruit_Sensor
  GyverTimer      -    https://github.com/AlexGyver/GyverLibs/tree/master/GyverTimer
  ------------------------------------------------------
*/

// ============ SETTINGS ============
// ----------- SERIAL -----------
#define SPEED_SERIAL 115200

// ------------ PINS -------------
#define RELAY_PIN 0

// -------- WiFi Manager --------
#define AC_SSID "Lamp"
#define AC_PASS ""

// ----------- EEPROM -----------
#define LENGHT_MQTT_SERVER 40
#define LENGHT_MQTT_PORT 6
#define LENGHT_MQTT_USER 20
#define LENGHT_MQTT_PASS 20
#define LENGHT_TOPIC_RELAY 40

#define START_ADDRESS_MQTT_SERVER         0
#define START_ADDRESS_MQTT_PORT           LENGHT_MQTT_SERVER + 2
#define START_ADDRESS_MQTT_USER           START_ADDRESS_MQTT_PORT + LENGHT_MQTT_PORT + 2
#define START_ADDRESS_MQTT_PASS           START_ADDRESS_MQTT_USER + LENGHT_MQTT_USER + 2
#define START_ADDRESS_TOPIC_RELAY         START_ADDRESS_MQTT_PASS + LENGHT_MQTT_PASS + 2
#define EEPROM_LENGTH                     START_ADDRESS_TOPIC_RELAY + LENGHT_TOPIC_RELAY
// =========== SETTINGS ===========


// ============= LIBS =============
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <PubSubClient.h>
#include <GyverTimer.h>
// ============= LIBS =============

ESP8266WebServer server(80);
WiFiClient       espClient;
PubSubClient     client(espClient);
GTimer           timerConnect(MS);
int              stateRelay;
String           MQTT_SERVER;
int              MQTT_PORT;
String           MQTT_USER;
String           MQTT_PASS;
String           TOPIC_RELAY;
bool             shouldSaveConfig;      //flag for saving data

void setup() {
  Serial.begin(SPEED_SERIAL);
  Serial.println();

  pinMode(RELAY_PIN, OUTPUT);
  shouldSaveConfig = false;
  digitalWrite(RELAY_PIN, HIGH);
  stateRelay = HIGH;

  beginEEPROM(EEPROM_LENGTH);
  delay(50);
  readConfig();
  wifiManagerInit();
  if (shouldSaveConfig) saveConfig();
  timerInit();
  wifiStartServer(saveConfigFromArgs, clearEEPROM, AC_SSID, getParamsSettings(), getParamsRelay());
}

void loop() {
  wifiHandleClient();
  if (!client.connected() && timerConnect.isReady())
    reconnect();
  else if (!client.connected() && !timerConnect.isEnabled()) {
    timerConnect.setInterval(500);
    timerConnect.start();
    client.publish(TOPIC_RELAY, String(stateRelay));
  }
  else if (client.connected()) {
    client.loop();
  }
}

void saveConfigCallback() {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void readConfig() {
  Serial.print(F("Reading config..."));
  MQTT_SERVER = getEEPROMData(START_ADDRESS_MQTT_SERVER);
  MQTT_PORT = getEEPROMData(START_ADDRESS_MQTT_PORT).toInt();
  MQTT_USER = getEEPROMData(START_ADDRESS_MQTT_USER);
  MQTT_PASS = getEEPROMData(START_ADDRESS_MQTT_PASS);
  TOPIC_RELAY = getEEPROMData(START_ADDRESS_TOPIC_RELAY);
  Serial.print(F("Readed! "));

  printVars("MQTT_SERVER", MQTT_SERVER);
  printVars("MQTT_PORT", String(MQTT_PORT));
  printVars("MQTT_USER", MQTT_USER);
  printVars("MQTT_PASS", MQTT_PASS);
  printVars("TOPIC_RELAY", TOPIC_RELAY);
  Serial.println();
}

void wifiManagerInit() {
  //set config save notify callback
  wifiSetSaveConfigCallback(saveConfigCallback);

  wifiSetDebugOutput(true);

  WiFiManagerParameter custom_space = wifiAddParameter("<p></p>");
  WiFiManagerParameter custom_text = wifiAddParameter("<span>MQTT Settings:</span>");
  WiFiManagerParameter custom_mqtt_server = wifiAddParameter("server", "mqtt server", MQTT_SERVER.c_str(), LENGHT_MQTT_SERVER);
  WiFiManagerParameter custom_mqtt_port = wifiAddParameter("port", "mqtt port", String(MQTT_PORT).c_str(), LENGHT_MQTT_PORT);
  WiFiManagerParameter custom_mqtt_user = wifiAddParameter("user", "mqtt user", MQTT_USER.c_str(), LENGHT_MQTT_USER);
  WiFiManagerParameter custom_mqtt_pass = wifiAddParameter("pass", "mqtt pass", MQTT_PASS.c_str(), LENGHT_MQTT_PASS);
  WiFiManagerParameter custom_space2 = wifiAddParameter("<p></p>");
  WiFiManagerParameter custom_text2 = wifiAddParameter("<span>Topic:</span>");
  WiFiManagerParameter custom_topic_relay = wifiAddParameter("topic_relay", "relay topic", TOPIC_RELAY.c_str(), LENGHT_TOPIC_RELAY);

  //fetches ssid and pass and tries to connect
  wifiAutoConnect(AC_SSID, AC_PASS);

  //read updated parameters
  MQTT_SERVER = custom_mqtt_server.getValue();
  MQTT_PORT = atoi(custom_mqtt_port.getValue());
  MQTT_USER = custom_mqtt_user.getValue();
  MQTT_PASS = custom_mqtt_pass.getValue();
  TOPIC_RELAY = custom_topic_relay.getValue();
}

void timerInit() {
  timerConnect.setInterval(500);
  timerConnect.start();
}

void reconnect() {
  // Loop until we're reconnected
  Serial.print(F("Attempting MQTT connection..."));
  // Attempt to connect
  client.set_server(MQTT_SERVER, MQTT_PORT);
  if (client.connect(MQTT::Connect(AC_SSID).set_auth(MQTT_USER, MQTT_PASS))) {
    Serial.println(F("connected"));
    client.set_callback(callback);
    client.subscribe(TOPIC_RELAY + "/set");
    timerConnect.stop();
  } else {
    Serial.println(F("failed"));
    timerConnect.setInterval(5000);
  }
}

void saveConfig() {
  Serial.println(F("------------------------------------"));
  Serial.print("Saving config...");
  saveDataInEEPROM(START_ADDRESS_MQTT_SERVER, MQTT_SERVER.c_str());
  saveDataInEEPROM(START_ADDRESS_MQTT_PORT, String(MQTT_PORT).c_str());
  saveDataInEEPROM(START_ADDRESS_MQTT_USER, MQTT_USER.c_str());
  saveDataInEEPROM(START_ADDRESS_MQTT_PASS, MQTT_PASS.c_str());
  saveDataInEEPROM(START_ADDRESS_TOPIC_RELAY, TOPIC_RELAY.c_str());
  Serial.print("Saved! ");

  printVars("MQTT_SERVER", MQTT_SERVER);
  printVars("MQTT_PORT", String(MQTT_PORT));
  printVars("MQTT_USER", MQTT_USER);
  printVars("MQTT_PASS", MQTT_PASS);
  printVars("TOPIC_RELAY", TOPIC_RELAY);
  Serial.println();
}

void saveConfigFromArgs() {
  MQTT_SERVER = server.arg("server");
  MQTT_PORT = server.arg("port").toInt();
  MQTT_USER = server.arg("user");
  MQTT_PASS = server.arg("pass");
  TOPIC_RELAY = server.arg("topic_relay");;
  saveConfig();
}

String getParamsSettings() {
  String params;
  params += "<p></p>";
  params += "<span>MQTT Settings:</span>";
  params += wifiAddParam("server", "mqtt server", MQTT_SERVER.c_str(), LENGHT_MQTT_SERVER);
  params += wifiAddParam("port", "mqtt port", String(MQTT_PORT).c_str(), LENGHT_MQTT_PORT);
  params += wifiAddParam("user", "mqtt user", MQTT_USER.c_str(), LENGHT_MQTT_USER);
  params += wifiAddParam("pass", "mqtt pass", MQTT_PASS.c_str(), LENGHT_MQTT_PASS);
  params += "<p></p>";
  params += "<span>Topiс:</span>";
  params += wifiAddParam("topic_relay", "relay topic", TOPIC_RELAY.c_str(), LENGHT_TOPIC_RELAY);
  return params;
}

String getParamsRelay() {
  String param;
  
  param += wifiAddNameList("Relay");
  param += wifiAddBodyList(((String) "State: " + String(stateRelay)).c_str());

  return param;
}

void callback(const MQTT::Publish& pub) {
  Serial.println("State: " + String(pub.topic()) + " -> " + pub.payload_string());

  stateRelay = pub.payload_string().toInt();
  digitalWrite(RELAY_PIN, stateRelay);
  client.publish(TOPIC_RELAY, String(stateRelay));
}
