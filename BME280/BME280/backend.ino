void useCSS() {
  Serial.println("Using CSS");

  DEBUG_LN("Use static file: dashboard.min.css");
  HttpServer.serveStatic("css/dashboard.min.css", SPIFFS, "/dashboard.min.css");  
  
  DEBUG_LN("Use static file: bootstrap.min.css");
  HttpServer.on("/css/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (isAP) {
      request->send(SPIFFS, "/bootstrap.min.css");
    } else {
      request->redirect("https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css");
    }
  });
  
  DEBUG_LN("Use static file: bootstrap.min.css.map");
  HttpServer.on("/css/bootstrap.min.css.map", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (isAP) {
      request->send(SPIFFS, "/bootstrap.min.css.map");
    } else {
      request->redirect("https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css.map");
    }
  });
}

void useJS() {
  Serial.println("Using JS");

  DEBUG_LN("Use static file: bootstrap.min.js");
  HttpServer.on("/js/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAP) {
      request->send(SPIFFS, "/bootstrap.min.js");
    } else {
      request->redirect("https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/js/bootstrap.min.js");
    }
  });

  DEBUG_LN("Use static file: Chart.min.js");
  HttpServer.on("/js/Chart.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAP) {
      request->send(SPIFFS, "/Chart.min.js");
    } else {
      request->redirect("https://www.chartjs.org/dist/2.9.3/Chart.min.js");
    }
  });

  DEBUG_LN("Use static file: jquery.min.js");
  HttpServer.on("/js/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    if (isAP) {
      request->send(SPIFFS, "/jquery.min.js");
    } else {
      request->redirect("https://ajax.googleapis.com/ajax/libs/jquery/2.1.1/jquery.min.js");
    }
  });
}

void usePages() {
  Serial.println("Using custom pages");
  // Index
  DEBUG_LN("Use route /, GET");
  HttpServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html", false, processor);
  });
  
  // WiFi
  DEBUG_LN("Use route /wifi, GET");
  HttpServer.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request) { 
    request->send(SPIFFS, "/wifi.html", "text/html", false, processor);
  });
  DEBUG_LN("Use route /wifi, POST");
  HttpServer.on("/wifi", HTTP_POST, [](AsyncWebServerRequest *request) { 
    handleWiFiSave(request);
  });
  
  // MQTT
  DEBUG_LN("Use route /mqtt, GET");
  HttpServer.on("/mqtt", HTTP_GET, [](AsyncWebServerRequest *request) { 
    request->send(SPIFFS, "/mqtt.html", "text/html", false, processor);
  });
  DEBUG_LN("Use route /mqtt, POST");
  HttpServer.on("/mqtt", HTTP_POST, [](AsyncWebServerRequest *request) { 
    handleMQTTSave(request);
  });
  
  // Control
  DEBUG_LN("Use route /sensor/values/get, GET");
  HttpServer.on("/sensor/values/get", HTTP_GET, [](AsyncWebServerRequest *request) { 
    handleValues(request);
  });

  // Reboot / Reset
  DEBUG_LN("Use route /reboot, GET");
  HttpServer.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request) { 
    handleReboot(request);
  });
  DEBUG_LN("Use route /reset, GET");
  HttpServer.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) { 
    handleReset(request);
  });
}
