String processor(const String& var) {
  String result = processor_index(var);
  if (result != "~") return result;

  result = processor_mqtt(var);
  if (result != "~") return result;

  result = processor_wifi(var);
  if (result != "~") return result;

  return String();
}

String processor_index(const String& var) {
  if (var == "NETWORK") {
    return WiFi.SSID();
  }
  else if (var == "MAC_ADDRESS") {
    return WiFi.macAddress();
  }
  else if (var == "IP") {
    return WiFi.localIP().toString();
  }
  else if (var == "CHIP_ID") {
    return String(ESP.getChipId());
  }
  else if (var == "FLASH_CHIP_ID") {
    return String(ESP.getFlashChipId());
  }
  else if (var == "FLASH_CHIP_SPEED") {
    return String(ESP.getFlashChipSpeed());
  }
  else if (var == "FLASH_CHIP_SIZE") {
    return String(ESP.getFlashChipSize());
  }
  else if (var == "FREE_HEAP_SIZE") {
    return String(ESP.getFreeHeap());
  }
  else if (var == "SENSOR") {
    return "BME280";
  }
  else if (var == "BOARD") {
    return "NodeMCU";
  }
  return "~";
}

String processor_mqtt(const String& var) {
  if (var == "MQTT_INTERVAL") {
    return String(mqtt_timeout_publish);
  }
  else if (var == "MQTT_INTERVAL_IN_SENCONDS") {
    return String(mqtt_timeout_publish / 1000);
  }
  else if (var == "CHECKED_USE_MQTT") {
    if (useMQTT)
      return "checked";
    return String();
  }
  else if (var == "CHECKED_USE_MQTT_AUTH") {
    if (useMQTTAuth)
      return "checked";
    return String();
  }
  else if (var == "VALUE_MQTT_SERVER") {
    return mqtt_server;
  }
  else if (var == "VALUE_MQTT_PORT") {
    return String(mqtt_port);
  }
  else if (var == "VALUE_MQTT_USER") {
    return mqtt_user;
  }
  else if (var == "VALUE_MQTT_PASS") {
    return mqtt_pass;
  }
  else if (var == "VALUE_MQTT_TIMEOUT_PUBLISH") {
    return String(mqtt_timeout_publish);
  }
  else if (var == "CHECKED_SEND_TEMP") {
    if (sendTemp)
      return "checked";
    return String();
  }
  else if (var == "VALUE_MQTT_TOPIC_TEMP") {
    return String(topic_temperature);
  }
  else if (var == "CHECKED_SEND_HUM") {
    if (sendHum)
      return "checked";
    return String();
  }
  else if (var == "VALUE_MQTT_TOPIC_HUM") {
    return String(topic_humidity);
  }
  else if (var == "CHECKED_SEND_PRESS") {
    if (sendPress)
      return "checked";
    return String();
  }
  else if (var == "VALUE_MQTT_TOPIC_PRESS") {
    return String(topic_pressure);
  }
  else if (var == "CHECKED_SEND_Altitude") {
    if (sendAltitude)
      return "checked";
    return String();
  }
  else if (var == "VALUE_MQTT_TOPIC_Altitude") {
    return String(topic_altitude);
  }
  return "~";
}

String processor_wifi(const String& var) {
  if (var == "CHECKED_EXISTING_WIFI") {
    if (useAP)
      return String();
    return "checked";
  }
  else if (var == "CHECKED_USE_AP") {
    if (useAP)
      return "checked";
    return String();
  }
  else if (var == "VALUE_WIFI_SSID") {
    return wifi_ssid;
  }
  else if (var == "VALUE_WIFI_PASSWORD") {
    return wifi_pass;
  }
  else if (var == "VALUE_AP_NAME") {
    return ap_ssid;
  }
  else if (var == "VALUE_AP_PASS") {
    return ap_pass;
  }

  return "~";
}
