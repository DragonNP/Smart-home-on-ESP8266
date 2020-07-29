/*
  Sketch for the project "RGB Led"
  The source code on GitHub:
  Author: DragonNP, 2020
  https://github.com/DragonNP/

  Libs link:
  ------------------------------------------------------
  ESP8266WiFi     -    https://github.com/esp8266/Arduino
  WiFiManager     -    https://github.com/tzapu/WiFiManager
  PubSubClient    -    https://github.com/ianscrivener/pubsubclient
  GyverTimer      -    https://github.com/AlexGyver/GyverLibs/tree/master/GyverTimer
  ------------------------------------------------------
*/

// ================== SETTINGS =================
// ----------- SERIAL -----------
#define SPEED_SERIAL 115200

// -------- WiFi Manager --------
#define AC_SSID "RGB Led"
#define AC_PASS ""

// ----------- EEPROM -----------
#define LENGHT_MQTT_SERVER             40
#define LENGHT_MQTT_PORT               6
#define LENGHT_MQTT_USER               20
#define LENGHT_MQTT_PASS               20
#define LENGHT_TOPIC_RGB_COLOR         20
#define LENGHT_TOPIC_MOTOR_STATE       20
#define LENGHT_TOPIC_MOTOR_DERECTION   20

#define START_ADDRESS_MQTT_SERVER              0
#define START_ADDRESS_MQTT_PORT                LENGHT_MQTT_SERVER + 2
#define START_ADDRESS_MQTT_USER                START_ADDRESS_MQTT_PORT + LENGHT_MQTT_PORT + 2
#define START_ADDRESS_MQTT_PASS                START_ADDRESS_MQTT_USER + LENGHT_MQTT_USER + 2
#define START_ADDRESS_TOPIC_RGB_COLOR          START_ADDRESS_MQTT_PASS + LENGHT_MQTT_PASS + 2
#define START_ADDRESS_TOPIC_MOTOR_STATE        START_ADDRESS_TOPIC_RGB_COLOR + LENGHT_TOPIC_RGB_COLOR + 2
#define START_ADDRESS_TOPIC_MOTOR_DERECTION    START_ADDRESS_TOPIC_MOTOR_STATE + LENGHT_TOPIC_MOTOR_STATE + 2
#define EEPROM_LENGTH                          START_ADDRESS_TOPIC_MOTOR_DERECTION + LENGHT_TOPIC_MOTOR_DERECTION + 2
// ===================== SETTINGS ================


// =================== LIBS ====================
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <GyverTimer.h>
// =================== LIBS ====================


ESP8266WebServer server(80);
WiFiClient       espClient;
PubSubClient     client(espClient);
GTimer           timerConnect(MS);
String           MQTT_SERVER;
int              MQTT_PORT;
String           MQTT_USER;
String           MQTT_PASS;
String           TOPIC_RGB_COLOR;
String           TOPIC_MOTOR_STATE;
String           TOPIC_MOTOR_DERECTION;
String           rgbColor, motorState, motorDerection;
bool             shouldSaveConfig;              //flag for saving data


void setup() {
  Serial.begin(SPEED_SERIAL);
  Serial.println();

  shouldSaveConfig = false;

  beginEEPROM(EEPROM_LENGTH);
  delay(50);
  readConfig();
  wifiManagerInit();
  if (shouldSaveConfig) saveConfig();
  timerInit();
  wifiStartServer(saveConfigFromArgs, clearEEPROM, AC_SSID, getParamsSettings(), getParams());
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
  }
}

void reconnect() {
  // Loop until we're reconnected
  Serial.print(F("Attempting MQTT connection..."));
  // Attempt to connect
  client.set_server(MQTT_SERVER, MQTT_PORT);
  if (client.connect(MQTT::Connect(AC_SSID).set_auth(MQTT_USER, MQTT_PASS))) {
    Serial.println(F("connected"));
    client.set_callback(callback);
    client.subscribe(TOPIC_RGB_COLOR);
    client.subscribe(TOPIC_MOTOR_STATE);
    client.subscribe(TOPIC_MOTOR_DERECTION);
    timerConnect.stop();
  } else {
    Serial.println(F("failed"));
    timerConnect.setInterval(5000);
  }
}

void wifiManagerInit() {
  //set config save notify callback
  wifiSetSaveConfigCallback(saveConfigCallback);

  wifiSetDebugOutput(true);

  WiFiManagerParameter custom_space                   = wifiAddParameter("<p></p>");
  WiFiManagerParameter custom_text                    = wifiAddParameter("<span>MQTT Settings:</span>");
  WiFiManagerParameter custom_mqtt_server             = wifiAddParameter("server", "mqtt server", MQTT_SERVER.c_str(), LENGHT_MQTT_SERVER);
  WiFiManagerParameter custom_mqtt_port               = wifiAddParameter("port", "mqtt port", String(MQTT_PORT).c_str(), LENGHT_MQTT_PORT);
  WiFiManagerParameter custom_mqtt_user               = wifiAddParameter("user", "mqtt user", MQTT_USER.c_str(), LENGHT_MQTT_USER);
  WiFiManagerParameter custom_mqtt_pass               = wifiAddParameter("pass", "mqtt pass", MQTT_PASS.c_str(), LENGHT_MQTT_PASS);
  WiFiManagerParameter custom_space2                  = wifiAddParameter("<p></p>");
  WiFiManagerParameter custom_text2                   = wifiAddParameter("<span>Topiсs:</span>");
  WiFiManagerParameter custom_topic_rgb               = wifiAddParameter("topic_rgb", "rgb topic", TOPIC_RGB_COLOR.c_str(), LENGHT_TOPIC_RGB_COLOR);
  WiFiManagerParameter custom_topic_motor_state       = wifiAddParameter("topic_motor_state", "motor state topic", TOPIC_MOTOR_STATE.c_str(), LENGHT_TOPIC_MOTOR_STATE);
  WiFiManagerParameter custom_topic_motor_derection   = wifiAddParameter("topic_motor_derection", "motor derection topic", TOPIC_MOTOR_DERECTION.c_str(), LENGHT_TOPIC_MOTOR_DERECTION);

  //fetches ssid and pass and tries to connect
  wifiAutoConnect(AC_SSID, AC_PASS);

  //read updated parameters
  MQTT_SERVER = custom_mqtt_server.getValue();
  MQTT_PORT = atoi(custom_mqtt_port.getValue());
  MQTT_USER = custom_mqtt_user.getValue();
  MQTT_PASS = custom_mqtt_pass.getValue();
  TOPIC_RGB_COLOR = custom_topic_rgb.getValue();
  TOPIC_MOTOR_STATE = custom_topic_motor_state.getValue();
  TOPIC_MOTOR_DERECTION = custom_topic_motor_derection.getValue();
}

void timerInit() {
  timerConnect.setInterval(500);
  timerConnect.start();
}

void readConfig() {
  Serial.print(F("Reading config..."));
  MQTT_SERVER = getEEPROMData(START_ADDRESS_MQTT_SERVER);
  MQTT_PORT = getEEPROMData(START_ADDRESS_MQTT_PORT).toInt();
  MQTT_USER = getEEPROMData(START_ADDRESS_MQTT_USER);
  MQTT_PASS = getEEPROMData(START_ADDRESS_MQTT_PASS);
  TOPIC_RGB_COLOR = getEEPROMData(START_ADDRESS_TOPIC_RGB_COLOR);
  TOPIC_MOTOR_STATE = getEEPROMData(START_ADDRESS_TOPIC_MOTOR_STATE);
  TOPIC_MOTOR_DERECTION = getEEPROMData(START_ADDRESS_TOPIC_MOTOR_DERECTION);
  Serial.print(F("Readed! "));

  printVars("MQTT_SERVER", MQTT_SERVER);
  printVars("MQTT_PORT", String(MQTT_PORT));
  printVars("MQTT_USER", MQTT_USER);
  printVars("MQTT_PASS", MQTT_PASS);
  printVars("TOPIC_RGB_COLOR", TOPIC_RGB_COLOR);
  printVars("TOPIC_MOTOR_STATE", TOPIC_MOTOR_STATE);
  printVars("TOPIC_MOTOR_DERECTION", TOPIC_MOTOR_DERECTION);
  Serial.println();
}

void saveConfig() {
  Serial.print(F("Saving config..."));
  saveDataInEEPROM(START_ADDRESS_MQTT_SERVER, MQTT_SERVER.c_str());
  saveDataInEEPROM(START_ADDRESS_MQTT_PORT, String(MQTT_PORT).c_str());
  saveDataInEEPROM(START_ADDRESS_MQTT_USER, MQTT_USER.c_str());
  saveDataInEEPROM(START_ADDRESS_MQTT_PASS, MQTT_PASS.c_str());
  saveDataInEEPROM(START_ADDRESS_TOPIC_RGB_COLOR, TOPIC_RGB_COLOR.c_str());
  saveDataInEEPROM(START_ADDRESS_TOPIC_MOTOR_STATE, TOPIC_MOTOR_STATE.c_str());
  saveDataInEEPROM(START_ADDRESS_TOPIC_MOTOR_DERECTION, TOPIC_MOTOR_DERECTION.c_str());
  Serial.print(F("Saved! "));

  printVars("MQTT_SERVER", MQTT_SERVER);
  printVars("MQTT_PORT", String(MQTT_PORT));
  printVars("MQTT_USER", MQTT_USER);
  printVars("MQTT_PASS", MQTT_PASS);
  printVars("TOPIC_RGB_COLOR", TOPIC_RGB_COLOR);
  printVars("TOPIC_MOTOR_STATE", TOPIC_MOTOR_STATE);
  printVars("TOPIC_MOTOR_DERECTION", TOPIC_MOTOR_DERECTION);
  Serial.println();
}

void saveConfigFromArgs() {
  MQTT_SERVER = server.arg("server");
  MQTT_PORT = server.arg("port").toInt();
  MQTT_USER = server.arg("user");
  MQTT_PASS = server.arg("pass");
  TOPIC_RGB_COLOR = server.arg("topic_rgb");
  TOPIC_MOTOR_STATE = server.arg("topic_motor_state");
  TOPIC_MOTOR_DERECTION = server.arg("topic_motor_derection");
  saveConfig();
}

void saveConfigCallback() {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void callback(const MQTT::Publish& pub) {
  // rgbColor, motorState, motorDerection
  if (String(pub.topic()) == String(TOPIC_RGB_COLOR))
    rgbColor = pub.payload_string();
  else if(String(pub.topic()) == String(TOPIC_MOTOR_STATE))
    motorState = pub.payload_string();
  else
    motorDerection = pub.payload_string();

  int pos = 0;
  String r, g, b;
  for(int i = 0; i < rgbColor.length(); i++) {
    if(rgbColor[i] == ',') {
      pos += 1;
      continue;
    }
    if(pos == 0) {
      r += rgbColor[i];
    }
    else if(pos == 1) {
      g += rgbColor[i];
    }
    else {
      b += rgbColor[i];
    }
  }
  
  Serial.println("r" + r + " g" + g + " b" + g + " s" + motorState + " d" + motorDerection);
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
  params += "<span>Topiсs:</span>";
  params += wifiAddParam("topic_rgb", "rgb topic", TOPIC_RGB_COLOR.c_str(), LENGHT_TOPIC_RGB_COLOR);
  params += wifiAddParam("topic_motor_state", "motor state topic", TOPIC_MOTOR_STATE.c_str(), LENGHT_TOPIC_MOTOR_STATE);
  params += wifiAddParam("topic_motor_derection", "motor derection topic", TOPIC_MOTOR_DERECTION.c_str(), LENGHT_TOPIC_MOTOR_DERECTION);
  return params;
}

String getParams() {
  String param;
  return param;
}
