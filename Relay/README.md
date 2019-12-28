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
	BLYNK_PRINT Serial        // Comment this out to disable prints and save space
	BLYNK_TOKEN "token"       // Blynk token

	// -------- WiFi Manager ---------
	AC_SSID "Rele"
	AC_PASS ""

	// ------------ PINS -------------
	BUTTON_PIN V2   // Pin button for on/off relay
	RELAY_PIN 2     // Pin relay
	
