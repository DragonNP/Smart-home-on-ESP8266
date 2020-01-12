#include "serverTemplate.h"

WiFiManager wifiManager;
String pageMain;
String pageSettings;
String pageReboot;
String pageReset;
String pageSaved;
String title;

void wifiAutoConnect(const char *ssid = "ESP " + ESP.getChipId(), const char *passwd = "");
void wifiInitServer(const char *title = "ESP " + ESP.getChipId());
void wifiStartServer(void (*saveFunc)(void), void (*resetFunc)(void), String paramInfo = "");

void wifiResetSettings() {
  wifiManager.resetSettings();
}

void wifiSetDebugOutput(bool isDebug) {
  wifiManager.setDebugOutput(isDebug);
}

void wifiSetSaveConfigCallback(void (*func)(void)) {
  wifiManager.setSaveConfigCallback(func);
}

void wifiAutoConnect(const char *ssid, const char *passwd) {
  wifiManager.autoConnect(ssid, passwd);
}

WiFiManagerParameter wifiAddParameter(const char *custom) {
  WiFiManagerParameter parameter(custom);
  wifiManager.addParameter(&parameter);

  pageSettings += custom;

  return parameter;
}

WiFiManagerParameter wifiAddParameter(const char *id, const char *placeholder, const char *defaultValue, int length) {
  WiFiManagerParameter parameter(id, placeholder, defaultValue, length);
  wifiManager.addParameter(&parameter);

  String pitem = FPSTR(SERVER_FORM_PARAM);
  pitem.replace("{i}", id);
  pitem.replace("{n}", id);
  pitem.replace("{l}", String(length).c_str());
  pitem.replace("{p}", placeholder);
  pitem.replace("{v}", defaultValue);
  pageSettings += pitem;

  return parameter;
}

void wifiInitServer(const char *title) {
  pageMain = FPSTR(SERVER_HEAD);
  pageMain.replace("{v}", title);
  pageMain += FPSTR(SERVER_STYLE);
  pageMain += FPSTR(SERVER_SCRIPT);
  pageMain += FPSTR(SERVER_HEAD_END);
  pageMain += FPSTR(SERVER_PORTAL_OPTIONS);
  pageMain += FPSTR(SERVER_END);

  pageSettings = FPSTR(SERVER_HEAD);
  pageSettings.replace("{v}", title);
  pageSettings += FPSTR(SERVER_STYLE);
  pageSettings += FPSTR(SERVER_SCRIPT);
  pageSettings += FPSTR(SERVER_HEAD_END);
  pageSettings += FPSTR(SERVER_FORM_START);

  pageReboot = FPSTR(SERVER_HEAD);
  pageReboot.replace("{v}", title);
  pageReboot += FPSTR(SERVER_STYLE);
  pageReboot += FPSTR(SERVER_SCRIPT);
  pageReboot += FPSTR(SERVER_HEAD_END);
  pageReboot += FPSTR(SERVER_REBOOT);
  pageReboot += FPSTR(SERVER_PORTAL_BACK);
  pageReboot += FPSTR(SERVER_END);

  pageReset = FPSTR(SERVER_HEAD);
  pageReset.replace("{v}", title);
  pageReset += FPSTR(SERVER_STYLE);
  pageReset += FPSTR(SERVER_SCRIPT);
  pageReset += FPSTR(SERVER_HEAD_END);
  pageReset += FPSTR(SERVER_RESET);
  pageReset += FPSTR(SERVER_END);

  pageSaved = FPSTR(SERVER_HEAD);
  pageSaved.replace("{v}", title);
  pageSaved += FPSTR(SERVER_STYLE);
  pageSaved += FPSTR(SERVER_SCRIPT);
  pageSaved += FPSTR(SERVER_HEAD_END);
  pageSaved += FPSTR(SERVER_SAVED);
  pageSaved += FPSTR(SERVER_PORTAL_BACK);
  pageSaved += FPSTR(SERVER_END);
}

void wifiStartServer(void (*saveFunc)(void), void (*resetFunc)(void), String paramInfo) {
  pageSettings += FPSTR(SERVER_FORM_END);
  pageSettings += FPSTR(SERVER_PORTAL_BACK);
  pageSettings += FPSTR(SERVER_END);

  server.on("/", []() {
    server.send(200, "text/html", pageMain);
  });
  server.on("/info", [paramInfo]() {
    handleInfo(paramInfo);
  });
  server.on("/settings", []() {
    server.send(200, "text/html", pageSettings);
  });
  server.on("/reboot", handleReboot);
  server.on("/reset", [resetFunc] {
    handleReset(resetFunc);
  });
  server.on("/save", [saveFunc]() {
    saveFunc();
    handleSaved();
  });
  server.begin();
}

void wifiHandleClient() {
  server.handleClient();
}

void handleInfo(String param) {
  String page = FPSTR(SERVER_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(SERVER_SCRIPT);
  page += FPSTR(SERVER_STYLE);
  page += FPSTR(SERVER_HEAD_END);
  page += F("<dl>");
  page += F("<dt>Chip ID</dt><dd>");
  page += ESP.getChipId();
  page += F("</dd>");
  page += F("<dt>Flash Chip ID</dt><dd>");
  page += ESP.getFlashChipId();
  page += F("</dd>");
  page += F("<dt>IDE Flash Size</dt><dd>");
  page += ESP.getFlashChipSize();
  page += F(" bytes</dd>");
  page += F("<dt>Real Flash Size</dt><dd>");
  page += ESP.getFlashChipRealSize();
  page += F(" bytes</dd>");
  page += F("<dt>Soft AP IP</dt><dd>");
  page += WiFi.softAPIP().toString();
  page += F("<dt>Local IP</dt><dd>");
  page += WiFi.localIP().toString();
  page += F("</dd>");
  page += F("<dt>Soft AP MAC</dt><dd>");
  page += WiFi.softAPmacAddress();
  page += F("</dd>");
  page += F("<dt>Station MAC</dt><dd>");
  page += WiFi.macAddress();
  page += F("</dd>");
  page += param;
  page += F("</dl>");
  page += FPSTR(SERVER_PORTAL_BACK);
  page += FPSTR(SERVER_END);

  server.sendHeader("Content-Length", String(page.length()));
  server.send(200, "text/html", page);
}

void handleReboot() {
  server.sendHeader("Content-Length", String(pageReboot.length()));
  server.send(200, "text/html", pageReboot);

  delay(1000);
  ESP.reset();
}

void handleReset(void (*resetFunc)(void)) {
  server.sendHeader("Content-Length", String(pageReset.length()));
  server.send(200, "text/html", pageReset);

  delay(1000);
  wifiResetSettings();
  resetFunc();
  ESP.reset();
}

void handleSaved() {
  server.sendHeader("Content-Length", String(pageSaved.length()));
  server.send(200, "text/html", pageSaved);

  delay(1000);
  ESP.reset();
}
