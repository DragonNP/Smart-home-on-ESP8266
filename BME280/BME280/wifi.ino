void WiFiStart() {
  WiFiConnect();
  WebStart();
}

void WiFiConnect() {  
  if (wifi_ssid == "" || wifi_pass == "" || useAP) {
    WiFi.disconnect();
    WiFi.softAP(ap_ssid, ap_pass);
    Serial.println("Using AP");
    
    isAP = true;
    
    DEBUG("Local IP: ");
    DEBUG_LN(WiFi.softAPIP());
  }
  else {
    Serial.print("Connecting to WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_pass);

    // Wating for connection
    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
      if (count >= 30) {
        Serial.println(" password or SSID is uncorrect, reset");
        delay(1000);
        clearWiFiSettings();
        ESP.reset();
      }
      
      delay(500);
      Serial.print(".");
      count++;
    }
    isAP = false;
    Serial.println(" Connected");

    DEBUG("IP address: ");
    DEBUG_LN(WiFi.localIP());
  }
}

void WebStart() {
  useCSS();
  useJS();
  usePages();
  
  HttpServer.begin();
  Serial.println("HTTP HttpServer started");
}
