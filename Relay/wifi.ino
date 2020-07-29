#include "serverTemplate.h"

WiFiManager wifiManager;

void wifiAutoConnect(const char *ssid = "ESP " + ESP.getChipId(), const char *passwd = "");
void wifiStartServer(void (*saveFunc)(void), void (*resetFunc)(void), String title = "ESP " + ESP.getChipId(), String paramsSettings = "", String paramInfo = "");

void handleMain(const char *title);
void handleInfo(String params);
void handleSettings(String params);
void handleReboot();
void handleReset(void (*resetFunc)(void));
void handleSaved();

void wifiAutoConnect(const char *ssid, const char *passwd) {
  wifiManager.autoConnect(ssid, passwd);
}

void wifiStartServer(void (*saveFunc)(void), void (*resetFunc)(void), const char *title, String paramsSettings, String paramsInfo) {
  server.on("/", [title]() {
    handleMain(title);
  });
  server.on("/info", [paramsInfo]() {
    handleInfo(paramsInfo);
  });
  server.on("/settings", [paramsSettings]() {
    handleSettings(paramsSettings);
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

WiFiManagerParameter wifiAddParameter(const char *custom) {
  WiFiManagerParameter parameter(custom);
  wifiManager.addParameter(&parameter);
  return parameter;
}

WiFiManagerParameter wifiAddParameter(const char *id, const char *placeholder, const char *defaultValue, int length) {
  WiFiManagerParameter parameter(id, placeholder, defaultValue, length);
  wifiManager.addParameter(&parameter);
  return parameter;
}

String wifiAddParam(const char *id, const char *placeholder, const char *value, int length) {
  String pitem = FPSTR(SERVER_FORM_PARAM);
  pitem.replace("{i}", id);
  pitem.replace("{n}", id);
  pitem.replace("{l}", String(length).c_str());
  pitem.replace("{p}", placeholder);
  pitem.replace("{v}", value);
  return pitem;
}

String wifiAddNameList(const char *name) {
  String pitem = FPSTR(SERVER_LIST_NAME);
  pitem.replace("{v}", name);
  return pitem;
}

String wifiAddBodyList(const char *body) {
  String pitem = FPSTR(SERVER_LIST_BODY);
  pitem.replace("{v}", body);
  return pitem;
}

void wifiSetSaveConfigCallback(void (*func)(void)) {
  wifiManager.setSaveConfigCallback(func);
}

void wifiSetDebugOutput(bool isDebug) {
  wifiManager.setDebugOutput(isDebug);
}

void wifiResetSettings() {
  wifiManager.resetSettings();
}

void wifiHandleClient() {
  server.handleClient();
}
