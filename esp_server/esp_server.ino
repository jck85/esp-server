// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright 2016-2025 Hristo Gochkov, Mathieu Carbou, Emil Muratov

//
// Show how to log the incoming request and response as a curl-like syntax
//

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WiFi.h>

#include "../secrets/wifi_creds.h"

static AsyncWebServer server(80);
static AsyncLoggingMiddleware requestLogger;

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  requestLogger.setEnabled(true);
  requestLogger.setOutput(Serial);
  server.addMiddleware(&requestLogger);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hello, world!");
  });

  server.begin();
}

void loop() {
  delay(100);
}