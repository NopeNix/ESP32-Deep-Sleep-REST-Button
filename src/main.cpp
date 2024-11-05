#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include <esp_sleep.h>

const int buttonPin = 4; // GPIO pin connected to button
bool previousButtonState = LOW;

#define RGB_BRIGHTNESS 200 // Change white brightness (max 255)

// Activate Build in LED
#ifdef RGB_BUILTIN
#undef RGB_BUILTIN
#endif
#define RGB_BUILTIN 10

// For 1sec and 2sec click
unsigned long startTime = 0; // When the button was pressed
unsigned long elapsedTime = 0; // How long the button was held

// Debug switch either triggered by 5sec+ hold or manually here
bool debug = true;

// Custom Includes
#include "WiFiCredentialsAndServerURL.h"
#include "rootca.h"
#include "mainFunctions.h"

//
// SETUP
//
void setup()
{
  if (esp_sleep_get_wakeup_cause() != 7)
  {
    neopixelWrite(RGB_BUILTIN,0,0,RGB_BRIGHTNESS); // LED Blue
  }
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);

  // Trigger HA if Device was woken up by PIN
  if (esp_sleep_get_wakeup_cause() == 7)
  {
    triggerHA();
  }
  neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
  if (debug == false) {
  goSleep();
  } else {
    Serial.println("[setup] Debug Enabled, staying awake");
    initializeOTA();  
  }
}

//
// MAIN LOOP
//
void loop()
{
  handleOTA();
  neopixelWrite(RGB_BUILTIN, 20, 0, 20); // Light Blue
  bool currentButtonState = digitalRead(buttonPin);
  if (previousButtonState == HIGH && currentButtonState == LOW) {
    Serial.println("[loop] Button Pressed!");
    triggerHA();
  }
  previousButtonState = currentButtonState;
  delay(50);
}

