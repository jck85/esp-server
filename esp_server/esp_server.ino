#include <Arduino.h>
#include <AsyncTCP.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include "lib/ArduinoJson-v7.4.2.h"
#include "secrets/wifi_creds.h"
#include "server/index.h"

AsyncWebServer server(8080);
AsyncWebSocket ws("/ws");
AsyncLoggingMiddleware requestLogger;

bool ledState = 0;
const int ledPin = 7;
String esp_ip = "";

void index_handler(AsyncWebServerRequest *request)
{
  request->send_P(200, "text/html", index_html);
}

void update_handler(AsyncWebServerRequest *request)
{

  JsonDocument doc;
  String response;

  int params = request->params();
  Serial.printf("Param count: %d\n", params);

  if (params == 0)
  {
    doc["msg"] = "OK";
  }
  else
  {
    for (int i = 0; i < params; i++)
    {
      const AsyncWebParameter *param = request->getParam(i);

      String name = param->name();
      String value = param->value();

      Serial.printf("PARAM: name: %s\tvalue: %s\n", name, value);
      doc[name] = value;
    }
  }

  serializeJson(doc, response);
  request->send(200, "application/json", response);
}

// Websockets
void ws_handler(AsyncWebServerRequest *request)
{
  JsonDocument doc;
  String response;

  doc["msg"] = "WS OK";

  serializeJson(doc, response);
  request->send(200, "application/json", response);
}

void ws_event(AsyncWebSocket *server,
              AsyncWebSocketClient *client,
              AwsEventType type,
              void *arg,
              uint8_t *data,
              size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;

  int client_id = client->id();
  const char *remote_ip = client->remoteIP().toString().c_str();

  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WS Client Connected: %d %s\n", client_id, client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WS Client Disconnected: %d\n", client->id());
    break;
  case WS_EVT_DATA:
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
      data[len] = 0;
      if (strcmp((char *)data, "toggle") == 0)
      {
        ledState = !ledState;
        ws.textAll(String(ledState));
      }
    }
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(250);
  }
  Serial.println();

  esp_ip = WiFi.localIP().toString();

  Serial.print("ESP IP:\t");
  Serial.println(esp_ip);

  // Logging info
  // requestLogger.setEnabled(true);
  // requestLogger.setOutput(Serial);
  // server.addMiddleware(&requestLogger);

  ws.onEvent(ws_event);
  server.addHandler(&ws);

  server.on("/", HTTP_GET, index_handler);

  server.on("/update", HTTP_GET, update_handler);

  server.on("/ws", HTTP_GET, ws_handler);

  server.begin();
}

void loop()
{
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
}
