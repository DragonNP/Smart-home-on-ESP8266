void mqttLoop() {
  if (!client.connected() && timerConnect.isReady()) {
    reconnect();
  }
  else if (!client.connected() && !timerConnect.isEnabled()) {
    DEBUG("Timer connect, set interval:");
    DEBUG_LN(MQTT_TIMEOUT_CONNECT);
    timerConnect.setInterval(MQTT_TIMEOUT_CONNECT);
    timerConnect.start();
    DEBUG_LN("Timer connect, start");
  }
  else if (client.connected() && !timerPublish.isEnabled() && (sendTemp || sendHum || sendPress || sendAltitude)) {
    DEBUG("Timer publish, set interval:");
    DEBUG(mqtt_timeout_publish);
    DEBUG_LN(", start");
    timerPublish.setInterval(mqtt_timeout_publish);
    timerPublish.start();

    DEBUG("Timer publish, stop, set interval:");
    DEBUG(mqtt_timeout_publish-200);
    DEBUG_LN(", reset");
    timerReader.stop();
    timerReader.setInterval(mqtt_timeout_publish-200);
    timerReader.reset();
    
  }
  else if (client.connected()) {
    client.loop();

    if (timerPublish.isReady()) {
      DEBUG_LN("Publishing to topics, if connected");
      if (sendTemp)     client.publish(topic_temperature, String(temp_value));
      if (sendHum)      client.publish(topic_humidity, String(hum_value));
      if (sendPress)    client.publish(topic_pressure, String(press_value));
      if (sendAltitude) client.publish(topic_altitude, String(altitude_value));
    }
  }
}

void reconnect() {
  Serial.print(F("Connecting to MQTT... "));
  // Attempt to connect
  client.set_server(mqtt_server, mqtt_port);
  bool result = false;
  if (useMQTTAuth) {
    result = client.connect(MQTT::Connect(ap_ssid).set_auth(mqtt_user, mqtt_pass));
    DEBUG("MQTT connect with auth ");
  }
  else {
    result = client.connect(ap_ssid);
    DEBUG("MQTT connect without auth ");
  }

  if (result) {
    Serial.println(F("Connected"));
    timerConnect.stop();
  } else {
    Serial.println(F("failed"));
    timerConnect.setInterval(5000);
  }
}
