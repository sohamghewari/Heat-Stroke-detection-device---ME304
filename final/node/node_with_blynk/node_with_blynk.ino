/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL37Q2IpY3Z"
#define BLYNK_TEMPLATE_NAME "myTemplate"
#define BLYNK_AUTH_TOKEN "aSvjo9ekKW5qzC4Pd9o9WjABllYvroB8"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "phone1";
char pass[] = "pleasehelp";
// char ssid[] = "VTPhone";
// char pass[] = "vithlove3";


BlynkTimer timer;
SoftwareSerial nodemcu(D6, D5);

const size_t CAPACITY = JSON_OBJECT_SIZE(16);
StaticJsonDocument<CAPACITY> doc;

JsonObject data = doc.as<JsonObject>();

float a, b, c;
uint8_t d;
const char* status = "Safe";
float result = 0;

void sendData() {
  if (round(a) != 0) {
    Blynk.virtualWrite(V1, a);
    Blynk.virtualWrite(V2, b);
    Blynk.virtualWrite(V3, c);
    Blynk.virtualWrite(V4, d);
    Blynk.virtualWrite(V5, status);
  }
}

void calculateStatus() {
  float ai = a, bi = b, ci = c;
  uint8_t di = d;
  // ai = 30, bi = 102, ci = 150;
  // di = 70;
  if (ai > 20 && bi > 90 && ci > 50 && di > 60) {
    float tempResult = 0.025 * (ai - 20) + 0.05 * (bi - 90) + 0.0066 * (ci - 50) + (1 - 0.025 * (di - 60));
    if (abs(tempResult - result) < 0.5) {
      if (result < 1) {
        status = "Safe";
      } else if (result < 2) {
        status = "Attention";
      } else if (result < 3) {
        status = "Warning";
      } else {
        status = "Danger";
      }
    }
    result = tempResult;
  } else {
    status = "Measuring...";
  }
}

void setup() {
  Serial.begin(9600);
  nodemcu.begin(14400);
  while (!Serial) continue;
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, sendData);
  timer.setInterval(2000L, calculateStatus);
}

void loop() {
  Blynk.run();
  timer.run();
  if (nodemcu.available() > 0) {
    deserializeJson(doc, nodemcu);
    data = doc.as<JsonObject>();

    a = data["AmbientTempInC"];
    if (round(a) != 0) {
      b = data["ObjectTempInF"];
      c = data["HeartRate"];
      d = data["SpO2"];
      Serial.print("Ambient Temperature in Celsius:  ");
      Serial.println(a);
      Serial.print("Object Temperature in Farh: ");
      Serial.println(b);
      Serial.print("Heart Rate:  ");
      Serial.println(c);
      Serial.print("SpO2:  ");
      Serial.println(d);
      Serial.println(status);
      Serial.println("-----------------------------------------");
      // serializeJson(doc, Serial);
      // Serial.println();
    }
  }
  doc.JsonDocument::clear();
}