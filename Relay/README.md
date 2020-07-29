![PROJECT_PHOTO](https://github.com/DragonNP/Smart-home-on-ESP8266/blob/master/Relay/schemes/scheme.png)
# Relay for smart home
* [Project description](#chapter-0)
* [Project folder](#chapter-1)
* [Settings in the code](#chapter-2)

<a id="chapter-0"></a>
## Project description
With this module, you can assemble a socket or lamp, for example

<a id="chapter-1"></a>
## Folders
- **libraries** - project libraries
- **firmware** - firmware for Arduino
- **schemes** - component wiring diagrams

<a id="chapter-2"></a>
## Settings in the code
	isDebug true

	// -------- WiFi Manager ---------
	AC_SSID "Relay"
	AC_PASS ""

	// ------------ PINS -------------
	RELAY_PIN 2                 	   // Pin relay

	// --------- MQTT SERVER ---------
	const char *mqtt_server = "server"; // Server name MQTT
	const int mqtt_port = 1883;         // Port for connecting to the MQTT server
	const char *mqtt_user = "login";    // Login from the server
	const char *mqtt_pass = "passwd";   // The password from the server
	TOPIK "topik"
