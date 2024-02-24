#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <esp_sleep.h>

// Include file with wifi Credentials
#include "WiFiCredentialsAndServerURL.h"

const int buttonPin = 4; // GPIO pin connected to button

#define RGB_BRIGHTNESS 200 // Change white brightness (max 255)

const char *test_root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIID4TCCAsmgAwIBAgIUZEF/1PeSIQrUuU0Rf0btIblQpj8wDQYJKoZIhvcNAQEL\n"
    "BQAwgYAxCzAJBgNVBAYTAkRFMQswCQYDVQQIDAJCWTENMAsGA1UECgwEaG9tZTEN\n"
    "MAsGA1UEBwwEaG9tZTEXMBUGA1UEAwwObmJnLm5vcGVuaXguZGUxDTALBgNVBAsM\n"
    "BGhvbWUxHjAcBgkqhkiG9w0BCQEWD2luZm9Abm9wZW5peC5kZTAeFw0yMjExMjcx\n"
    "NTE0MDZaFw0zMjExMjQxNTE0MDZaMIGAMQswCQYDVQQGEwJERTELMAkGA1UECAwC\n"
    "QlkxDTALBgNVBAoMBGhvbWUxDTALBgNVBAcMBGhvbWUxFzAVBgNVBAMMDm5iZy5u\n"
    "b3Blbml4LmRlMQ0wCwYDVQQLDARob21lMR4wHAYJKoZIhvcNAQkBFg9pbmZvQG5v\n"
    "cGVuaXguZGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCfQexI45rZ\n"
    "IHO5WfiKEmAilUTz1IdKxqaL/73VpS/mUMzGp5oifiC5vD1ecH2WLYIJ0jP/UBv+\n"
    "LPpFZVqSSnhNaBa0nqjX5FPeIXgGESsT/i2CP0fmdepkP7iubDk0mFIQmMV9cur6\n"
    "sSQ35zHHbJQl3Xw99yEcHw35pyzUliOBNv36P8lTT8PIpkBo4EKnPoKISAZhKUf0\n"
    "YoXBbpBE2f6WNEXWKpzTif1GrdUuooImLQTR1a3yThfKZNoSGkcvKEsoARE8JtZy\n"
    "eLacvL06csh5y9CeXwDNhog3Ew5f89QSE+DrBy8MV1HpDtEDitnKc1kz/5bSXv93\n"
    "Y1p68vaozij5AgMBAAGjUTBPMAsGA1UdDwQEAwIF4DATBgNVHSUEDDAKBggrBgEF\n"
    "BQcDATArBgNVHREEJDAigg5uYmcubm9wZW5peC5kZYIQKi5uYmcubm9wZW5peC5k\n"
    "ZTANBgkqhkiG9w0BAQsFAAOCAQEAUDvNcpIUm7qQBFR2c8K1y1AE3vnO4NNdPzY0\n"
    "OwS7CqHDWLNrRbdM0itoiDrAOfnvr3pZf2+22uWx3SE4OZxKdZ6PIb8pmA7xCToT\n"
    "jhtDTeotB1Faj9dsCLqpbco2M+gRR0ySqucssKtV6UkTAtgnsS/O5G5FaTq1qWvL\n"
    "7KYJ8ZYsjvkjdOsqjUak5gkUUXleXCbeMGSotCp4+r14QIIFqYCS/3rt/5g2PoJi\n"
    "4993KVCGxVkKWmFwLPisOrXQ1a4nAGZDFNDM6wRyzHG6YpuiX0iC4Irjm7NrrUX1\n"
    "K26t2WCWKA/8rVQMcMAQUsUpwhfRZU7wMaO8eEE1oUY0RqAR/w==\n"
    "-----END CERTIFICATE-----\n";

// Activate Build in LED
#ifdef RGB_BUILTIN
#undef RGB_BUILTIN
#endif
#define RGB_BUILTIN 10

//
// SETUP
//
void setup()
{
  neopixelWrite(RGB_BUILTIN,0,0,RGB_BRIGHTNESS); // LED Blue
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  connectToWiFi();
  Serial.print("[Setup] Connected to SSID ");
  Serial.println(ssid);

  // Trigger HA if Device was woken up by PIN
  if (esp_sleep_get_wakeup_cause() == 7)
  {
    triggerHA();
  }
  neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
  goSleep();
}

//
// MAIN LOOP
//
void loop()
{
}

//
// Sleep function
//
void goSleep()
{
  // esp_sleep_enable_gpio_wakeup();
  Serial.println("[goSleep] Going to deep sleep");
  esp_deep_sleep_enable_gpio_wakeup((1ULL << buttonPin), ESP_GPIO_WAKEUP_GPIO_LOW);
  esp_deep_sleep_start();
}

//
// TRIGGER HomeAssistant
//
void triggerHA()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("[triggerHA] Wifi not connected, connecting...");
    connectToWiFi();
  }
  else
  {
    Serial.println("[triggerHA] Wifi is already connected");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("[triggerHA] Sending HTTP Request");
    sendHTTPRequest();
  }
}

//
// CONNECT TO WIFI
//
void connectToWiFi()
{
  Serial.println("[connectToWiFi] Connecting to WiFi");
  WiFi.begin(ssid, password);
  bool isOdd = true;

  while (WiFi.status() != WL_CONNECTED)
  {
    if (isOdd == true)
    {
      neopixelWrite(RGB_BUILTIN,RGB_BRIGHTNESS,RGB_BRIGHTNESS,0); // LED Yellow
    }
    else
    {
      neopixelWrite(RGB_BUILTIN, 0, 0, 0); // LED Off / black
    }
    isOdd = !isOdd;
    Serial.println("[connectToWiFi] Connecting...");
    delay(50);
  }

  Serial.printf("[connectToWiFi] Own IP       : ");
  Serial.println(WiFi.localIP());
  Serial.printf("[connectToWiFi] DNS          : ");
  Serial.println(WiFi.dnsIP());
  Serial.printf("[connectToWiFi] GTW          : ");
  Serial.println(WiFi.gatewayIP());
  Serial.printf("[connectToWiFi] Own Hostname : ");
  Serial.println(WiFi.getHostname());
  Serial.printf("[connectToWiFi] Own MAC      : ");
  Serial.println(WiFi.macAddress());
  Serial.println("[connectToWiFi] Connected to WiFi");
}

//
// HTTP REQUEST
//
void sendHTTPRequest()
{
  WiFiClientSecure client;

  client.setCACert(test_root_ca);

  Serial.println("\n[sendHTTPRequest] Starting connection to server...");
  if (!client.connect(serverUrl, 443))
  {
    Serial.println("[sendHTTPRequest] Connection failed!");
    neopixelWrite(RGB_BUILTIN,0,RGB_BRIGHTNESS,0); // LED Red
    delay(1000);
  }
  else
  {
    Serial.println("[sendHTTPRequest] Connected to server!");
    // Make a HTTP request:
    client.print(String("PUT /api/webhook/shutdown-button-bYIjAjAIoBtHdZ1tJz14Nopj HTTP/1.1\r\n") +
                 "Host: ha.nbg.nopenix.de\r\n" +
                 "Connection: close\r\n\r\n");

    while (client.connected())
    {
      String line = client.readStringUntil('\n');
      if (line == "\r")
      {
        Serial.println("[sendHTTPRequest] headers received");
        neopixelWrite(RGB_BUILTIN,RGB_BRIGHTNESS,0,0); // LED Green
        delay(1000);
        break;
      }
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available())
    {
      char c = client.read();
      Serial.write(c);
    }
    client.stop();
  }
}