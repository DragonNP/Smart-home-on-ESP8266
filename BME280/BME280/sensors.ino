void readValues() {
  temp_value = bme.readTemperature();
  hum_value = bme.readHumidity();
  press_value = bme.readPressure() / 100.0F * HPA_TO_MMHG;
  altitude_value = bme.readAltitude(SEALEVELPRESSURE_HPA);

  debug_readValues();
}
