# Temperature sensor for smart home
* [Project description](#chapter-0)
* [Project folder](#chapter-1)
* [Settings in the code](#chapter-2)

<a id="chapter-0"></a>
## Project description
I present you the first module - the Temperature Sensor! The temperature can be viewed in the Blynk app!

<a id="chapter-1"></a>
## Folders
- **libraries** - project libraries
- **firmware** - firmware for Arduino
- **schemes** - component wiring diagrams

<a id="chapter-2"></a>
## Settings in the code
	BLYNK_PRINT Serial        // Comment this out to disable prints and save space
	INTERVAL_TIMER 1000       // Set the time (in milliseconds) to poll the temperature sensor
	BLYNK_TOKEN "token"  // Blynk token

	// -------- WiFi Manager ---------
	AC_SSID "Temperature sensor"
	AC_PASS ""

	// ------------ PINS -------------
	EMP_SENSOR_INPUT_PIN 2   	// Pin on which the temperature sensor is installed
	TEMP_SENSOR_OUTPUT_PIN V1 	// The temperature is sent to this pin
	
