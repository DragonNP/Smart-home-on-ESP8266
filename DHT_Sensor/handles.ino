#include "serverTemplate.h"

void handleMain(const char *title) {
  String pageMain = FPSTR(SERVER_HEAD);
  pageMain.replace("{v}", title);
  pageMain += FPSTR(SERVER_STYLE);
  pageMain += FPSTR(SERVER_HEAD_END);
  pageMain += FPSTR(SERVER_PORTAL_OPTIONS);
  pageMain += FPSTR(SERVER_END);

  server.sendHeader("Content-Length", String(pageMain.length()));
  server.send(200, "text/html", pageMain);
}

void handleInfo(String params) {
  String page = FPSTR(SERVER_HEAD);
  page.replace("{v}", "Информация");
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
  page += F("</dd>");
  page += F("<dt>Local IP</dt><dd>");
  page += WiFi.localIP().toString();
  page += F("</dd>");
  page += F("<dt>Soft AP MAC</dt><dd>");
  page += WiFi.softAPmacAddress();
  page += F("</dd>");
  page += F("<dt>Station MAC</dt><dd>");
  page += WiFi.macAddress();
  page += F("</dd>");
  page += params;
  page += F("</dl>");
  page += FPSTR(SERVER_PORTAL_BACK);
  page += FPSTR(SERVER_END);

  server.sendHeader("Content-Length", String(page.length()));
  server.send(200, "text/html", page);
}

void handleSettings(String params) {
  String pageSettings = FPSTR(SERVER_HEAD);
  pageSettings.replace("{v}", "Настройки");
  pageSettings += FPSTR(SERVER_STYLE);
  pageSettings += FPSTR(SERVER_HEAD_END);
  pageSettings += FPSTR(SERVER_FORM_START);
  pageSettings += params;
  pageSettings += FPSTR(SERVER_FORM_END);
  pageSettings += FPSTR(SERVER_PORTAL_BACK);
  pageSettings += FPSTR(SERVER_END);

  server.sendHeader("Content-Length", String(pageSettings.length()));
  server.send(200, "text/html", pageSettings);
}

void handleReboot() {
  String pageReboot = FPSTR(SERVER_HEAD);
  pageReboot.replace("{v}", "Перезагрузка");
  pageReboot += FPSTR(SERVER_STYLE);
  pageReboot += FPSTR(SERVER_HEAD_END);
  pageReboot += FPSTR(SERVER_REBOOT);
  pageReboot += FPSTR(SERVER_PORTAL_BACK);
  pageReboot += FPSTR(SERVER_END);

  server.sendHeader("Content-Length", String(pageReboot.length()));
  server.send(200, "text/html", pageReboot);

  delay(1000);
  ESP.reset();
}

void handleReset(void (*resetFunc)(void)) {
  String pageReset = FPSTR(SERVER_HEAD);
  pageReset.replace("{v}", "Сброс настроек");
  pageReset += FPSTR(SERVER_STYLE);;
  pageReset += FPSTR(SERVER_HEAD_END);
  pageReset += FPSTR(SERVER_RESET);
  pageReset += FPSTR(SERVER_END);

  server.sendHeader("Content-Length", String(pageReset.length()));
  server.send(200, "text/html", pageReset);

  delay(1000);
  wifiResetSettings();
  resetFunc();
  ESP.reset();
}

void handleSaved() {
  String pageSaved = FPSTR(SERVER_HEAD);
  pageSaved.replace("{v}", "Сохранено");
  pageSaved += FPSTR(SERVER_STYLE);
  pageSaved += FPSTR(SERVER_HEAD_END);
  pageSaved += FPSTR(SERVER_SAVED);
  pageSaved += FPSTR(SERVER_PORTAL_BACK);
  pageSaved += FPSTR(SERVER_END);

  server.sendHeader("Content-Length", String(pageSaved.length()));
  server.send(200, "text/html", pageSaved);

  delay(1000);
  ESP.reset();
}
