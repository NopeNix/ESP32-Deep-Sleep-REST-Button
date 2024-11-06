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
// CONNECT TO WIFI
//
void connectToWiFi()
{
  Serial.println("[connectToWiFi] Connecting to WiFi");
  WiFi.begin(ssid, password);
  bool isOdd = true;
  int8_t counter = 1;

  while (WiFi.status() != WL_CONNECTED)
  {
    if (counter == 50)
    {
      neopixelWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0); // LED Red
      Serial.println("[connectToWiFi] ERROR! Cannot connect to Wifi :( going to sleep...");
      delay(3000);
      neopixelWrite(RGB_BUILTIN, 0, 0, 0); // LED Off / black
      goSleep();
    }

    if (isOdd == true)
    {
      neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, RGB_BRIGHTNESS, 0); // LED Yellow
    }
    else
    {
      neopixelWrite(RGB_BUILTIN, 0, 0, 0); // LED Off / black
    }
    isOdd = !isOdd;
    Serial.printf("[connectToWiFi] Connecting... ");
    Serial.println(counter);
    counter++;
    delay(100);
  }
  Serial.printf("[connectToWiFi] Connected to : ");
  Serial.println(WiFi.SSID());
  Serial.printf("[connectToWiFi] Channel      : ");
  Serial.println(WiFi.channel());
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
void sendHTTPRequest(String endpoint)
{
  WiFiClientSecure client;

  client.setCACert(test_root_ca);

  Serial.println("\n[sendHTTPRequest] Starting connection to server...");
  if (!client.connect(serverUrl, 443, 5000))
  {
    Serial.println("[sendHTTPRequest] Connection failed!");
    neopixelWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0); // LED Red
    delay(5000);
    neopixelWrite(RGB_BUILTIN, 0, 0, 0); // LED Off / black
  }
  else
  {
    Serial.println("[sendHTTPRequest] Connected to server!");
    // Make a HTTP request:
    String host = serverUrl;
    client.print(String("PUT /api/webhook/" + endpoint + " HTTP/1.1\r\n") +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    while (client.connected())
    {
      String line = client.readStringUntil('\n');
      if (line == "\r")
      {
        Serial.println("[sendHTTPRequest] headers received");
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0); // LED Green
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

//
// TRIGGER HomeAssistant
//
void triggerHA()
{
  startTime = millis();
  bool pressedshort = true;
  bool pressedonesec = false;
  bool pressedtwosec = false;
  bool pressedfivesec = false;

  String webhookIDToUse = webhookIDshort;

  Serial.println("[triggerHA] Button pressed...");
  while (digitalRead(buttonPin) == LOW)
  {
    elapsedTime = millis() - startTime;
    if (elapsedTime > 10000)
    {
      if (!pressedfivesec)
      {
        pressedfivesec = true;
        Serial.println("[triggerHA] Button pressed for ten seconds...");

        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(50);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
        delay(50);
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(50);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
        delay(50);
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(50);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
        delay(50);
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(50);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
        delay(50);
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(50);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
        delay(50);
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(50);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
        delay(50);
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(50);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
        delay(50);
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(50);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
        delay(50);
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(50);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
        delay(50);
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(50);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
        delay(50);
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(50);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
        delay(50);
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(50);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
        delay(50);
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(50);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
      }
    }
    else if (elapsedTime > 2000 and elapsedTime < 9999)
    {
      if (!pressedtwosec)
      {
        pressedtwosec = true;
        Serial.println("[triggerHA] Button pressed for two seconds...");
        webhookIDToUse = webhookIDtwosec;
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(200);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
        delay(200);
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(200);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
      }
    }
    else if (elapsedTime > 1000)
    {
      if (!pressedonesec)
      {
        pressedonesec = true;
        Serial.println("[triggerHA] Button pressed for one second...");
        webhookIDToUse = webhookIDonesec;
        neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, (RGB_BRIGHTNESS / 2), 0); // LED Orange
        delay(200);
        neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
      }
    }
    delay(10);
  }
  Serial.print("[triggerHA] Button released, has been held for ");
  Serial.print(elapsedTime);
  Serial.println("ms");

  if (WiFi.status() != WL_CONNECTED and pressedfivesec != true)
  {
    Serial.println("[triggerHA] Wifi not connected, connecting...");
    connectToWiFi();
  }
  else
  {
    Serial.println("[triggerHA] Wifi is already connected");
  }

  if (WiFi.status() == WL_CONNECTED and pressedfivesec == false)
  {
    Serial.println("[triggerHA] Sending HTTP Request");
    sendHTTPRequest(webhookIDToUse);
  }
  else if (pressedfivesec == true)
  {
    debug = !debug;
    Serial.printf("[triggerHA] Setting debug to ");
    Serial.println(debug);
    if (debug == false) 
    {
      Serial.printf("[triggerHA] Restarting ESP... ");
      ESP.restart();
    }
  }
}

//
// OTA Function
//
void initializeOTA()
{
  // Connect to Wi-Fi
  connectToWiFi();

  // Configure OTA
  ArduinoOTA.onStart([]()
                     {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else {
            type = "filesystem";
        }
        Serial.println("[initializeOTA] Start updating " + type); });

  ArduinoOTA.onEnd([]()
                   { Serial.println("\n[initializeOTA] End"); });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("\n[initializeOTA] Progress: %u%%\r", (progress / (total / 100))); });

  ArduinoOTA.onError([](ota_error_t error)
                     {
        Serial.printf("[initializeOTA] Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("[initializeOTA] Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("[initializeOTA] Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("[initializeOTA] Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("[initializeOTA] Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("[initializeOTA] End Failed");
        } });

  ArduinoOTA.begin();
  Serial.println("[initializeOTA] Ready for OTA updates");
}

//
// OTA Handle
//
void handleOTA()
{
  ArduinoOTA.handle();
}