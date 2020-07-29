void handleWiFiSave(AsyncWebServerRequest *request) {
  Serial.println("POST: /wifi");
  debug_handleWiFiSave(request);

  if (request->arg("ssid") != "") wifi_ssid = request->arg("ssid");
  wifi_pass = request->arg("pass");
  if (request->arg("nameAP") != "") ap_ssid = request->arg("nameAP");
  ap_pass = request->arg("passAP");
  if (request->arg("connectionType") == "ap") useAP = true;
  else useAP = false;
  
  saveWiFi();
  saveAP();
  
  DEBUG_LN("Sending: ok");
  request->send(200, "ok");
}

void handleMQTTSave(AsyncWebServerRequest *request) {
  Serial.println("POST: /mqtt");
  debug_handleMQTTSave(request);

  // MQTT
  if (request->arg("server") != "") mqtt_server = request->arg("server");
  if (request->arg("port") != "") mqtt_port = request->arg("port").toInt();
  mqtt_user = request->arg("user");
  mqtt_pass = request->arg("pass");
  if (request->arg("timeout_publish") != "") mqtt_timeout_publish = request->arg("timeout_publish").toInt();
  
  if (request->arg("useMQTT") == "on") useMQTT = true;
  else useMQTT = false;
  if (request->arg("useAuth") == "on") useMQTTAuth = true;
  else useMQTTAuth = false;

  // Topics
  topic_temperature = request->arg("topic_temp");
  topic_humidity = request->arg("topic_hum");
  topic_pressure = request->arg("topic_press");
  topic_altitude = request->arg("topic_altitude");
  if (request->arg("sendTemp") == "on") sendTemp = true;
  else sendTemp = false;
  if (request->arg("sendHum") == "on") sendHum = true;
  else sendHum = false;
  if (request->arg("sendPress") == "on") sendPress = true;
  else sendPress = false;
  if (request->arg("sendAltitude") == "on") sendAltitude = true;
  else sendAltitude = false;

  saveMQTT();
  saveTopics();
  request->send(200, "text/html", "ok");
}

void handleValues(AsyncWebServerRequest *request) {
  Serial.println("GET: /sensor/values/get");
  
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  
  String tempValue = String(temp_value);
  String humValue = String(hum_value);
  String pressValue = String(press_value);
  String altitudeValue = String(altitude_value);
  String time = String(timeClient.getHours()) + ":" + String(timeClient.getMinutes());
  
  String json = "{";
  json += "\"temperature\": \"" + tempValue + "\", ";
  json += "\"humidity\": \"" + humValue + "\", ";
  json += "\"pressure\": \"" + pressValue + "\", ";
  json += "\"altitude\": \"" + altitudeValue + "\", ";
  json += "\"time\": \"" + time + "\"";
  json += "}";

  DEBUG("Sending: ");
  DEBUG(json);
  request->send(200, "text/plane", json);
}

void handleReboot(AsyncWebServerRequest *request) {
  Serial.println("GET: /reboot");
  
  DEBUG_LN("Sending: ok");
  request->send(200, "ok");
  
  ESP.restart();
}

void handleReset(AsyncWebServerRequest *request) {
  Serial.println("GET: /reset");
  
  DEBUG_LN("Sending: ok");
  request->send(200, "ok");
  
  clearEEPROM();
  ESP.restart();
}
