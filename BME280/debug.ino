void debug_readConfig() {
  DEBUG("Readed vars: ");
  DEBUG("WiFi SSID:" + wifi_ssid);
  DEBUG(", WiFi pass:" + wifi_pass);
  DEBUG(", AP SSID:" + ap_ssid);
  DEBUG(", AP pass:" + ap_pass);
  DEBUG(", Use AP:");
  if (useAP) DEBUG("true");
  else DEBUG("false");
  DEBUG(", use MQTT broker:");
  if (useMQTT) DEBUG("true");
  else DEBUG("false");
  DEBUG(", MQTT server:" + mqtt_server);
  DEBUG(", MQTT port:" + mqtt_port);
  DEBUG(", Use MQTT auth:");
  if (useMQTTAuth) DEBUG("true");
  else DEBUG("false");
  DEBUG(", MQTT user:" + mqtt_user);
  DEBUG(", MQTT pass:" + mqtt_pass);
  DEBUG(", MQTT timeout publish:" + mqtt_timeout_publish);
  DEBUG(", Topic temperature:" + topic_temperature);
  DEBUG(", Use topic temperature:");
  if (sendTemp) DEBUG("true");
  else DEBUG("false");
  DEBUG(", Topic humidity:" + topic_humidity);
  DEBUG(", Use topic humidity:");
  if (sendHum) DEBUG("true");
  else DEBUG("false");
  DEBUG(", Topic pressure:" + topic_pressure);
  DEBUG(", Use topic pressure:");
  if (sendPress) DEBUG("true");
  else DEBUG("false");
  DEBUG(", Topic altitude:" + topic_altitude);
  DEBUG(", Use topic altitude:");
  if (sendAltitude) DEBUG_LN("true");
  else DEBUG_LN("false");
}

void debug_handleWiFiSave(AsyncWebServerRequest *request) {
  DEBUG("Recived data: ");
  DEBUG("Count:");
  DEBUG(request->args());
  DEBUG(", SSID WiFi:");
  DEBUG(request->arg("ssid"));
  DEBUG(", Pass WiFi:");
  DEBUG(request->arg("pass"));
  DEBUG(", SSID AP:");
  DEBUG(request->arg("nameAP"));
  DEBUG(", Pass AP:");
  DEBUG(request->arg("passAP"));
  DEBUG(", WiFi Mode:");
  DEBUG_LN(request->arg("connectionType"));
}

void debug_handleMQTTSave(AsyncWebServerRequest *request) {
  DEBUG("Recived data: ");
  DEBUG("Count:");
  DEBUG(request->args());
  DEBUG(", Use MQTT:");
  DEBUG(request->arg("useMQTT"));
  DEBUG(", Use Auth:");
  DEBUG(request->arg("useAuth"));
  DEBUG(", Server:");
  DEBUG(request->arg("server"));
  DEBUG(", Port:");
  DEBUG(request->arg("port"));
  DEBUG(", User:");
  DEBUG(request->arg("user"));
  DEBUG(", Pass:");
  DEBUG(request->arg("pass"));
  DEBUG(", Timeout publish:");
  DEBUG(request->arg("timeout_publish"));
  DEBUG(", Send temp:");
  DEBUG(request->arg("sendTemp"));
  DEBUG(", Topic temp:");
  DEBUG(request->arg("topic_temp"));
  DEBUG(", Send hum:");
  DEBUG(request->arg("sendHum"));
  DEBUG(", Topic hum:");
  DEBUG(request->arg("topic_hum"));
  DEBUG(", Send press:");
  DEBUG(request->arg("sendPress"));
  DEBUG(", Topic press:");
  DEBUG(request->arg("topic_press"));
  DEBUG(", Send altitude:");
  DEBUG(request->arg("sendAltitude"));
  DEBUG(", Topic altitude:");
  DEBUG_LN(request->arg("topic_altitude"));
}

void debug_readValues() {
  DEBUG("Readed values: ");
  DEBUG("temperature: ");
  DEBUG(temp_value);
  DEBUG(", humidity:");
  DEBUG(hum_value);
  DEBUG(", pressure:");
  DEBUG(press_value);
  DEBUG(", altitude:");
  DEBUG_LN(altitude_value);
}
