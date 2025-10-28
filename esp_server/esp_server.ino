#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WiFi.h>

#include "ArduinoJson-v7.4.2.h"
#include "../secrets/wifi_creds.h"
#include "index.h"

const char *param_1 = "gpio";
const char *param_2 = "state";

static AsyncWebServer server(80);
static AsyncLoggingMiddleware requestLogger;

// Process index_html and add template
String processor(const String &var)
{
  Serial.println(var);
  if (var == "%BUTTON_TEMPALTE%")
  {
    String button_string = "<button>button</button>";
    return button_string;
  }

  return String();
}

void index_handler(AsyncWebServerRequest *request)
{
  request->send_P(200, "text/html", index_html);
}

void update_handler(AsyncWebServerRequest *request)
{

  String gpio_num;
  String gpio_state;

  JsonDocument doc;

  // GET /update?gpio=<gpio_num>&state=<gpio_state>
  if (request->hasParam("gpio") && request->hasParam("state"))
  {
    gpio_num = request->getParam(param_1)->value();
    gpio_state = request->getParam(param_2)->value();
    digitalWrite(gpio_num.toInt(), gpio_state.toInt());
  }
  else
  {
    gpio_num = "null";
    gpio_state = "null";
  }

  doc["gpio_num"] = gpio_num;
  doc["gpio_state"] = gpio_state;
  serializeJsonPretty(doc, Serial);

  String response;
  serializeJson(doc, response);
  request->send(200, "application/json", response);
}

void setup()
{

  Serial.begin(115200);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  requestLogger.setEnabled(true);
  requestLogger.setOutput(Serial);

  server.addMiddleware(&requestLogger);

  // Route for root / web page
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  //           { request->send_P(200, "text/html", index_html, processor); });

  server.on("/", HTTP_GET, index_handler);

  // Send a GET request to <ESP_IP>/update?output=<msg_1>&state=<inputMessage2>
  // server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
  //           {
  //             String gpio_num;
  //             String gpio_state;

  //             JsonDocument doc;

  //             // GET /update?gpio=<gpio_num>&state=<gpio_state>
  //             if (request->hasParam("gpio") && request->hasParam("state")) {
  //               gpio_num = request->getParam(param_1)->value();
  //               gpio_state = request->getParam(param_2)->value();
  //               digitalWrite(gpio_num.toInt(), gpio_state.toInt());
  //             }
  //             else {
  //               gpio_num = "null";
  //               gpio_state = "null";
  //             }

  //               doc["gpio_num"] = gpio_num;
  //               doc["gpio_state"] = gpio_state;
  //               serializeJsonPretty(doc, Serial);

  //               String response;
  //               serializeJson(doc, response);
  //               request->send(200, "application/json", response); });

  server.on("/update", HTTP_GET, update_handler);

  server.begin();
}

void loop()
{
  // delay(100);
}