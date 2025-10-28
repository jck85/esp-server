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
// String processor(const String &var)
// {
//   Serial.println(var);
//   if (var == "%BUTTON_TEMPALTE%")
//   {
//     String button_string = "<button>button</button>";
//     return button_string;
//   }

//   return String();
// }

void index_handler(AsyncWebServerRequest *request)
{
  request->send_P(200, "text/html", index_html);
}

void update_handler(AsyncWebServerRequest *request)
{

  String gpio_num;
  String gpio_state;
  JsonDocument doc;

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

  serializeJsonPretty(doc, Serial);
  Serial.println();

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
    Serial.println("Connecting to WiFi..");
  }

  Serial.print("ESP IP:\t");
  Serial.println(WiFi.localIP());

  // Logging info
  // requestLogger.setEnabled(true);
  // requestLogger.setOutput(Serial);
  // server.addMiddleware(&requestLogger);

  server.on("/", HTTP_GET, index_handler);

  server.on("/update", HTTP_GET, update_handler);

  server.begin();
}

void loop()
{
}