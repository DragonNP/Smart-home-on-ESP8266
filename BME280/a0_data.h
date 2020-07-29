// ============ VARIABLES ===========
// WiFi
String wifi_ssid;
String wifi_pass;
// AP WiFi
String ap_ssid = AP_SSID_DEFAULT;
String ap_pass = AP_PASS_DEFAULT;
bool   isAP = false;
bool   useAP = false;
// MQTT
String mqtt_server;
int    mqtt_port;
String mqtt_user;
String mqtt_pass;
int    mqtt_timeout_publish;
bool   useMQTT = false;
bool   useMQTTAuth = false;
// Topics
String topic_temperature;
String topic_humidity;
String topic_pressure;
String topic_altitude;
bool   sendTemp = false;
bool   sendHum = false;
bool   sendPress = false;
bool   sendAltitude = false;

float temp_value = 0;
float hum_value = 0;
float press_value = 0;
float altitude_value = 0;

// Timers
GTimer timerConnect(MS);
GTimer timerPublish(MS);
GTimer timerReader(MS);
// WiFi
WiFiClient espClient;
// Server
AsyncWebServer HttpServer(80);
// MQTT
PubSubClient client(espClient);
// BME280 Sensor
Adafruit_BME280 bme;
// Date
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
// ============ VARIABLES ===========
