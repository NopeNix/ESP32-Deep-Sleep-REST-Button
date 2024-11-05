# ESP32-Deep-Sleep-REST-Button
Button that deep sleeps until button is pressed (<1sec, <2sec, >2sec) then sends REST and goes to Sleep again

## How to Install
a Settings file is needed within the root directory of this repo with the name ```WiFiCredentialsAndServerURL.h``` wih the following content:
```c
const char *ssid = ""; // WiFi Name
const char *password = ""; // WiFi Password
const char *serverUrl = ""; // HomeAssistant URI e.g. "homeassistant.yourdomain.com"

const char *webhookIDshort = ""; // Homeassistant Webhook ID (not URL!) for a press shorter than one second 
const char *webhookIDonesec = ""; // Homeassistant Webhook ID (not URL!) for a press between 1-2 seconds
const char *webhookIDtwosec = ""; // Homeassistant Webhook ID (not URL!) for a press longer than 2 seconds
```

## Debug and OTA
Can be enabled by either setting ```debug = true;``` in ```main.cpp```  or by pressing and holding the Button for 5 Seconds