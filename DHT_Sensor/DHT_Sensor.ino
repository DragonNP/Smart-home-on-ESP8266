/*
  Sketch for the project "DHT Sensor"
  The source code on GitHub: https://github.com/DragonNP/Smart-home-on-ESP8266/tree/master/DHT_Sensor
  Author: DragonNP, 2020
  https://github.com/DragonNP/

  Libs link:
  ------------------------------------------------------
  ESP8266WiFi     -    https://github.com/esp8266/Arduino
  WiFiManager     -    https://github.com/tzapu/WiFiManager
  PubSubClient    -    https://github.com/ianscrivener/pubsubclient
  GyverTimer      -    https://github.com/AlexGyver/GyverLibs/tree/master/GyverTimer
  Adafruit_Sensor -    https://github.com/adafruit/Adafruit_Sensor
  DHT             -    https://github.com/adafruit/DHT-sensor-library
  ------------------------------------------------------
*/

// ================== SETTINGS =================
// ----------- SERIAL -----------
#define SPEED_SERIAL 115200

// ------------- DHT ------------
#define DHT_PIN    2
#define DHT_TYPE   DHT11     // DHT 11
//#define DHT_TYPE DHT22     // DHT 22 (AM2302)
//#define DHT_TYPE DHT21     // DHT 21 (AM2301)

// -------- WiFi Manager --------
#define AC_SSID "DHT sensor"
#define AC_PASS ""

// ----------- EEPROM -----------
#define LENGHT_MQTT_SERVER         40
#define LENGHT_MQTT_PORT           6
#define LENGHT_MQTT_USER           20
#define LENGHT_MQTT_PASS           20
#define LENGHT_SEND_TIME           6
#define LENGHT_TOPIC_TEMPERATURE   40
#define LENGHT_TOPIC_HUMIDITY      40

#define START_ADDRESS_MQTT_SERVER         0
#define START_ADDRESS_MQTT_PORT           LENGHT_MQTT_SERVER + 2
#define START_ADDRESS_MQTT_USER           START_ADDRESS_MQTT_PORT + LENGHT_MQTT_PORT + 2
#define START_ADDRESS_MQTT_PASS           START_ADDRESS_MQTT_USER + LENGHT_MQTT_USER + 2
#define START_ADDRESS_SEND_TIME           START_ADDRESS_MQTT_PASS + LENGHT_MQTT_PASS + 2
#define START_ADDRESS_TOPIC_TEMPERATURE   START_ADDRESS_SEND_TIME + LENGHT_SEND_TIME + 2
#define START_ADDRESS_TOPIC_HUMIDITY      START_ADDRESS_TOPIC_TEMPERATURE + LENGHT_TOPIC_TEMPERATURE + 2
#define EEPROM_LENGTH                     START_ADDRESS_TOPIC_HUMIDITY + LENGHT_TOPIC_HUMIDITY
// ===================== SETTINGS ================


// =================== LIBS ====================
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <GyverTimer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
// =================== LIBS ====================


ESP8266WebServer server(80);
WiFiClient       espClient;
PubSubClient     client(espClient);
GTimer           timer(MS);
GTimer           timerConnect(MS);
String           MQTT_SERVER;
int              MQTT_PORT;
String           MQTT_USER;
String           MQTT_PASS;
int              SEND_TIME;
String           TOPIC_TEMPERATURE;
String           TOPIC_HUMIDITY;
bool             shouldSaveConfig;              //flag for saving data
DHT_Unified      dht(DHT_PIN, DHT_TYPE);


void setup() {
  Serial.begin(SPEED_SERIAL);
  Serial.println();

  shouldSaveConfig = false;

  beginEEPROM(EEPROM_LENGTH);
  delay(50);
  readConfig();
  wifiManagerInit();
  if (shouldSaveConfig) saveConfig();
  dht.begin();
  timerInit();
  wifiStartServer(saveConfigFromArgs, clearEEPROM, AC_SSID, getParamsSettings(), getParamsDHT());
}

void loop() {
  wifiHandleClient();
  if (!client.connected() && timerConnect.isReady())
    reconnect();
  else if (!client.connected() && !timerConnect.isEnabled()) {
    timerConnect.setInterval(500);
    timerConnect.start();
  }
  else if (client.connected()) {
    client.loop();
    if (timer.isReady()) sendTemperature();
  }
}

void reconnect() {
  // Loop until we're reconnected
  Serial.print(F("Attempting MQTT connection..."));
  // Attempt to connect
  client.set_server(MQTT_SERVER, MQTT_PORT);
  if (client.connect(MQTT::Connect(AC_SSID).set_auth(MQTT_USER, MQTT_PASS))) {
    Serial.println(F("connected"));
    timerConnect.stop();
  } else {
    Serial.println(F("failed"));
    timerConnect.setInterval(5000);
  }
}

void sendTemperature() {
  sensors_event_t event;
  
  // Get temperature
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
    Serial.println(F("Error reading temperature!"));
  else
    client.publish(TOPIC_TEMPERATURE, String(event.temperature));

  // Get humidity
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
    Serial.println(F("Error reading humidity!"));
  else
    client.publish(TOPIC_HUMIDITY, String(event.relative_humidity));
}

void wifiManagerInit() {
  //set config save notify callback
  wifiSetSaveConfigCallback(saveConfigCallback);

  wifiSetDebugOutput(true);

  WiFiManagerParameter custom_space       = wifiAddParameter("<p></p>");
  WiFiManagerParameter custom_text        = wifiAddParameter("<span>MQTT Settings:</span>");
  WiFiManagerParameter custom_mqtt_server = wifiAddParameter("server", "mqtt server", MQTT_SERVER.c_str(), LENGHT_MQTT_SERVER);
  WiFiManagerParameter custom_mqtt_port   = wifiAddParameter("port", "mqtt port", String(MQTT_PORT).c_str(), LENGHT_MQTT_PORT);
  WiFiManagerParameter custom_mqtt_user   = wifiAddParameter("user", "mqtt user", MQTT_USER.c_str(), LENGHT_MQTT_USER);
  WiFiManagerParameter custom_mqtt_pass   = wifiAddParameter("pass", "mqtt pass", MQTT_PASS.c_str(), LENGHT_MQTT_PASS);
  WiFiManagerParameter custom_send_time   = wifiAddParameter("time", "send time", String(SEND_TIME).c_str(), LENGHT_SEND_TIME);
  WiFiManagerParameter custom_space2      = wifiAddParameter("<p></p>");
  WiFiManagerParameter custom_text2       = wifiAddParameter("<span>Topiсs:</span>");
  WiFiManagerParameter custom_topic_temp  = wifiAddParameter("topic_temp", "temperature topic", TOPIC_TEMPERATURE.c_str(), LENGHT_TOPIC_TEMPERATURE);
  WiFiManagerParameter custom_topic_hum   = wifiAddParameter("topic_hum", "humidity topic", TOPIC_HUMIDITY.c_str(), LENGHT_TOPIC_HUMIDITY);

  //fetches ssid and pass and tries to connect
  wifiAutoConnect(AC_SSID, AC_PASS);

  //read updated parameters
  MQTT_SERVER = custom_mqtt_server.getValue();
  MQTT_PORT = atoi(custom_mqtt_port.getValue());
  MQTT_USER = custom_mqtt_user.getValue();
  MQTT_PASS = custom_mqtt_pass.getValue();
  SEND_TIME = atoi(custom_send_time.getValue());
  TOPIC_TEMPERATURE = custom_topic_temp.getValue();
  TOPIC_HUMIDITY = custom_topic_hum.getValue();
}

void timerInit() {
  timerConnect.setInterval(500);
  timerConnect.start();
  //timer.setInterval(sensor.min_delay / 1000);
  timer.setInterval(SEND_TIME);
  timer.start();
}

void readConfig() {
  Serial.print(F("Reading config..."));
  MQTT_SERVER = getEEPROMData(START_ADDRESS_MQTT_SERVER);
  MQTT_PORT = getEEPROMData(START_ADDRESS_MQTT_PORT).toInt();
  MQTT_USER = getEEPROMData(START_ADDRESS_MQTT_USER);
  MQTT_PASS = getEEPROMData(START_ADDRESS_MQTT_PASS);
  SEND_TIME =  getEEPROMData(START_ADDRESS_SEND_TIME).toInt();
  TOPIC_TEMPERATURE = getEEPROMData(START_ADDRESS_TOPIC_TEMPERATURE);
  TOPIC_HUMIDITY = getEEPROMData(START_ADDRESS_TOPIC_HUMIDITY);
  Serial.print(F("Readed! "));

  printVars("MQTT_SERVER", MQTT_SERVER);
  printVars("MQTT_PORT", String(MQTT_PORT));
  printVars("MQTT_USER", MQTT_USER);
  printVars("MQTT_PASS", MQTT_PASS);
  printVars("SEND_TIME", String(SEND_TIME));
  printVars("TOPIC_TEMPERATURE", TOPIC_TEMPERATURE);
  printVars("TOPIC_HUMIDITY", TOPIC_HUMIDITY);
  Serial.println();
}

void saveConfig() {
  Serial.print(F("Saving config..."));
  saveDataInEEPROM(START_ADDRESS_MQTT_SERVER, MQTT_SERVER.c_str());
  saveDataInEEPROM(START_ADDRESS_MQTT_PORT, String(MQTT_PORT).c_str());
  saveDataInEEPROM(START_ADDRESS_MQTT_USER, MQTT_USER.c_str());
  saveDataInEEPROM(START_ADDRESS_MQTT_PASS, MQTT_PASS.c_str());
  saveDataInEEPROM(START_ADDRESS_SEND_TIME, String(SEND_TIME).c_str());
  saveDataInEEPROM(START_ADDRESS_TOPIC_TEMPERATURE, TOPIC_TEMPERATURE.c_str());
  saveDataInEEPROM(START_ADDRESS_TOPIC_HUMIDITY, TOPIC_HUMIDITY.c_str());
  Serial.print(F("Saved! "));

  printVars("MQTT_SERVER", MQTT_SERVER);
  printVars("MQTT_PORT", String(MQTT_PORT));
  printVars("MQTT_USER", MQTT_USER);
  printVars("MQTT_PASS", MQTT_PASS);
  printVars("SEND_TIME", String(SEND_TIME));
  printVars("TOPIC_TEMPERATURE", TOPIC_TEMPERATURE);
  printVars("TOPIC_HUMIDITY", TOPIC_HUMIDITY);
  Serial.println();
}

void saveConfigFromArgs() {
  MQTT_SERVER = server.arg("server");
  MQTT_PORT = server.arg("port").toInt();
  MQTT_USER = server.arg("user");
  MQTT_PASS = server.arg("pass");
  SEND_TIME = server.arg("time").toInt();
  TOPIC_TEMPERATURE = server.arg("topic_temp");
  TOPIC_HUMIDITY = server.arg("topic_hum");
  saveConfig();
}

void saveConfigCallback() {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

String getParamsSettings() {
  String params;
  params += "<p></p>";
  params += "<span>MQTT Settings:</span>";
  params += wifiAddParam("server", "mqtt server", MQTT_SERVER.c_str(), LENGHT_MQTT_SERVER);
  params += wifiAddParam("port", "mqtt port", String(MQTT_PORT).c_str(), LENGHT_MQTT_PORT);
  params += wifiAddParam("user", "mqtt user", MQTT_USER.c_str(), LENGHT_MQTT_USER);
  params += wifiAddParam("pass", "mqtt pass", MQTT_PASS.c_str(), LENGHT_MQTT_PASS);
  params += wifiAddParam("time", "send time", String(SEND_TIME).c_str(), LENGHT_SEND_TIME);
  params += "<p></p>";
  params += "<span>Topiсs:</span>";
  params += wifiAddParam("topic_temp", "temperature topic", TOPIC_TEMPERATURE.c_str(), LENGHT_TOPIC_TEMPERATURE);
  params += wifiAddParam("topic_hum", "humidity topic", TOPIC_HUMIDITY.c_str(), LENGHT_TOPIC_HUMIDITY);
  return params;
}

String getParamsDHT() {
  String param;
  sensor_t sensor;
  
  dht.temperature().getSensor(&sensor);
  param += wifiAddNameList("Temperatue");
  param += wifiAddBodyList(((String) "Sensor Type: " + sensor.name).c_str());
  param += wifiAddBodyList(((String) "Driver Ver: " + sensor.version).c_str());
  param += wifiAddBodyList(((String) "Unique ID: " + sensor.sensor_id).c_str());
  param += wifiAddBodyList(((String) "Max Value:  " + sensor.max_value + "°C").c_str());
  param += wifiAddBodyList(((String) "Min Value:  " + sensor.min_value + "°C").c_str());
  param += wifiAddBodyList(((String) "Resolution:  " + sensor.resolution + "°C").c_str());

  dht.humidity().getSensor(&sensor);
  param += wifiAddNameList("Humidity");
  param += wifiAddBodyList(((String) "Sensor Type: " + sensor.name).c_str());
  param += wifiAddBodyList(((String) "Driver Ver: " + sensor.version).c_str());
  param += wifiAddBodyList(((String) "Unique ID: " + sensor.sensor_id).c_str());
  param += wifiAddBodyList(((String) "Max Value:  " + sensor.max_value + "%").c_str());
  param += wifiAddBodyList(((String) "Min Value:  " + sensor.min_value + "%").c_str());
  param += wifiAddBodyList(((String) "Resolution:  " + sensor.resolution + "%").c_str());

  return param;
}
