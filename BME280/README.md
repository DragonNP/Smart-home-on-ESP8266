![PROJECT_PHOTO](https://github.com/DragonNP/Smart-home-on-ESP8266/blob/master/BME28/schemes/scheme.png)
# BME280 for smart home
* [Project description](#chapter-0)
* [Project folder](#chapter-1)
* [Settings in the code](#chapter-2)

<a id="chapter-0"></a>
## Project description
The BME280 sensor will become an assistant in a smart home.
It can change: temperature, pressure, humidity and altitude.
And in conjunction with bootstrap, you can watch the readings change. WebInterface is friendly and functional

<a id="chapter-1"></a>
## Folders
- **BME280** - firmware for NodeMCU
- **schemes** - component wiring diagrams
- **webInterface** - sample web pages

<a id="chapter-2"></a>
## Settings in the code
	DEBUG_ENABLE
	TIMEZONE     +3

	// -------------- MQTT --------------
	MQTT_TIMEOUT_CONNECT         5000
	MQTT_TIMEOUT_PUBLISH_DEFAULT 3000

	// ------------- BME280 -------------
	SEALEVELPRESSURE_HPA (1013.25)
	HPA_TO_MMHG          0.7500638

	//-------------- SERIAL -------------
	SPEED_SERIAL 115200

	// -------------- WiFi --------------
	AP_SSID_DEFAULT "BME280"
	AP_PASS_DEFAULT ""
