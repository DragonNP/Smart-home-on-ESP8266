void beginEEPROM() {
  Serial.println("Beginning EEPROM");
  EEPROM.begin(EEPROM_LENGTH);
}


void readConfig() {
  Serial.print(F("Reading config... "));
  // WiFi
  wifi_ssid = getEEPROMData(START_ADDRESS_WIFI_SSID);
  wifi_pass = getEEPROMData(START_ADDRESS_WIFI_PSSD);
  
  // AP
  ap_ssid = getEEPROMData(START_ADDRESS_AP_NAME);
  ap_pass = getEEPROMData(START_ADDRESS_AP_PSSD);
  if (getEEPROMData(START_ADDRESS_AP_USE) == "1") useAP = true;
  else useAP = false;
  
  // MQTT
  mqtt_server = getEEPROMData(START_ADDRESS_MQTT_SERVER);
  mqtt_port = getEEPROMData(START_ADDRESS_MQTT_PORT).toInt();
  mqtt_user = getEEPROMData(START_ADDRESS_MQTT_USER);
  mqtt_pass = getEEPROMData(START_ADDRESS_MQTT_PASS);
  mqtt_timeout_publish = getEEPROMData(START_ADDRESS_TIMEOUT_PUBLISH).toInt();
  if (getEEPROMData(START_ADDRESS_MQTT_AUTH) == "1") useMQTTAuth = true;
  else useMQTTAuth = false;
  if (getEEPROMData(START_ADDRESS_MQTT) == "1") useMQTT = true;
  else useMQTT = false;
  
  // Topics
  topic_temperature = getEEPROMData(START_ADDRESS_TOPIC_TEMP);
  topic_humidity = getEEPROMData(START_ADDRESS_TOPIC_HUM);
  topic_pressure = getEEPROMData(START_ADDRESS_TOPIC_PRESS);
  topic_altitude = getEEPROMData(START_ADDRESS_TOPIC_ALTITUDE);
  if (getEEPROMData(START_ADDRESS_USE_TOPIC_TEMP) == "1") sendTemp = true;
  else sendTemp = false;
  if (getEEPROMData(START_ADDRESS_USE_TOPIC_HUM) == "1") sendHum = true;
  else sendHum = false;
  if (getEEPROMData(START_ADDRESS_USE_TOPIC_PRESS) == "1") sendPress = true;
  else sendPress = false;
  if (getEEPROMData(START_ADDRESS_USE_TOPIC_ALTITUDE) == "1") sendAltitude = true;
  else sendAltitude = false;

  if (ap_ssid == "") ap_ssid = AP_SSID_DEFAULT;
  
  Serial.println("Readed");
  debug_readConfig();
}

void saveWiFi() {
  Serial.print("Saving WiFi config... ");
  saveDataInEEPROM(START_ADDRESS_WIFI_SSID, wifi_ssid.c_str());
  saveDataInEEPROM(START_ADDRESS_WIFI_PSSD, wifi_pass.c_str());
  Serial.println("Saved!");
}

void saveAP() {
  Serial.print("Saving AP config... ");
  saveDataInEEPROM(START_ADDRESS_AP_NAME, ap_ssid.c_str());
  saveDataInEEPROM(START_ADDRESS_AP_PSSD, ap_pass.c_str());
  if (useAP) saveDataInEEPROM(START_ADDRESS_AP_USE, String("1").c_str());
  else saveDataInEEPROM(START_ADDRESS_AP_USE, String("0").c_str());
  Serial.println("Saved!");
}

void saveMQTT() {
  Serial.print("Saving MQTT config... ");
  saveDataInEEPROM(START_ADDRESS_MQTT_SERVER, mqtt_server.c_str());
  saveDataInEEPROM(START_ADDRESS_MQTT_PORT, String(mqtt_port).c_str());
  saveDataInEEPROM(START_ADDRESS_MQTT_USER, mqtt_user.c_str());
  saveDataInEEPROM(START_ADDRESS_MQTT_PASS, mqtt_pass.c_str());
  saveDataInEEPROM(START_ADDRESS_TIMEOUT_PUBLISH, String(mqtt_timeout_publish).c_str());
  
  if (useMQTTAuth) saveDataInEEPROM(START_ADDRESS_MQTT_AUTH, String("1").c_str());
  else saveDataInEEPROM(START_ADDRESS_MQTT_AUTH, String("0").c_str());
  
  if (useMQTT) saveDataInEEPROM(START_ADDRESS_MQTT, String("1").c_str());
  else saveDataInEEPROM(START_ADDRESS_MQTT, String("0").c_str());
  Serial.println("Saved!");
}

void saveTopics() {
  Serial.print("Saving MQTT topics config... ");
  saveDataInEEPROM(START_ADDRESS_TOPIC_TEMP, topic_temperature.c_str());
  saveDataInEEPROM(START_ADDRESS_TOPIC_HUM, topic_humidity.c_str());
  saveDataInEEPROM(START_ADDRESS_TOPIC_PRESS, topic_pressure.c_str());
  saveDataInEEPROM(START_ADDRESS_TOPIC_ALTITUDE, topic_altitude.c_str());

  // Temp
  if (sendTemp) saveDataInEEPROM(START_ADDRESS_USE_TOPIC_TEMP, String("1").c_str());
  else saveDataInEEPROM(START_ADDRESS_USE_TOPIC_TEMP, String("0").c_str());
  // Hum
  if (sendHum) saveDataInEEPROM(START_ADDRESS_USE_TOPIC_HUM, String("1").c_str());
  else saveDataInEEPROM(START_ADDRESS_USE_TOPIC_HUM, String("0").c_str());
  // Press
  if (sendPress) saveDataInEEPROM(START_ADDRESS_USE_TOPIC_PRESS, String("1").c_str());
  else saveDataInEEPROM(START_ADDRESS_USE_TOPIC_PRESS, String("0").c_str());
  // Altitude
  if (sendAltitude) saveDataInEEPROM(START_ADDRESS_USE_TOPIC_ALTITUDE, String("1").c_str());
  else saveDataInEEPROM(START_ADDRESS_USE_TOPIC_ALTITUDE, String("0").c_str());
  Serial.println("Saved!");
}


void clearEEPROM() {
  Serial.print("Clearing EEPROM... ");
  for (int i = 0 ; i <= EEPROM_LENGTH ; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  Serial.print("Cleared! from 0 to ");
  Serial.println(EEPROM_LENGTH);
}

void clearWiFiSettings() {
  Serial.print("Clearing WiFi settings... ");
  clearDataInEEPROM(START_ADDRESS_WIFI_SSID, LENGHT_WIFI_SSID);
  clearDataInEEPROM(START_ADDRESS_WIFI_PSSD, LENGHT_WIFI_PSSD);
  Serial.println("Cleared!");
}

void saveDataInEEPROM(int fieldstartaddress, const char* stringdata) {
  // The event name is a string whose length varies
  // The eeprom location at the start address holds the length of the data

  int data_length = strlen(stringdata);

  int dataStartAddress = fieldstartaddress + 1;
  int dataEndAddress = dataStartAddress + data_length;
  int totalValidCharacters = 0;

  for (int addr = 0; addr < data_length; addr++) {
    EEPROM.write(addr + dataStartAddress, stringdata[addr]);
    EEPROM.commit();
    totalValidCharacters = totalValidCharacters + 1;
  }
  //Update the length of the valid characters
  EEPROM.write(fieldstartaddress, totalValidCharacters);
  EEPROM.commit();
}

void clearDataInEEPROM(int fieldstartaddress, int lenghtAddress) {
  for (int i = fieldstartaddress ; i <= fieldstartaddress + lenghtAddress; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

String getEEPROMData(int fieldstartaddress) {
  // The event name is a string whose length varies
  //The eeprom location at the start address holds the length of the data

  int data_length = EEPROM.read(fieldstartaddress);
  int dataStartAddress = fieldstartaddress + 1;
  int dataEndAddress = dataStartAddress + data_length;
  String dataString;

  for (int addr = 0; addr < data_length; addr++) {
    char characterInEEPROM = EEPROM.read(addr + dataStartAddress);
    dataString = dataString + characterInEEPROM;
  }

  return dataString;
}
