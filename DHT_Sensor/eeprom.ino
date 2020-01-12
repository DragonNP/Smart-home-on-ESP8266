void beginEEPROM(int length) {
  EEPROM.begin(length);
}

void clearEEPROM() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

int saveDataInEEPROM(int fieldstartaddress, const char* stringdata) {
  // The event name is a string whose length varies
  //The eeprom location at the start address holds the length of the data

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

  return totalValidCharacters + 1;
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
