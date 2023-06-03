#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "MAX30100_PulseOximeter.h"
#include <ArduinoJson.h>
//Arduino to NodeMCU Lib
#include <SoftwareSerial.h>
#include <StreamUtils.h>

#define REPORTING_PERIOD_MS 1000

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation

PulseOximeter pox;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(5, 6);

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected() {
  // Serial.println("beat!");
}

void setup() {
  Serial.begin(9600);

  Serial.print("Initializing pulse oximeter..");

  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FAILED");
    while(true);
  } else {
    Serial.println("SUCCESS");
  }

  // The default current for the IR LED is 50mA and it could be changed
  //   by uncommenting the following line. Check MAX30100_Registers.h for all the
  //   available options.
  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);

  Serial.print("Arduino MLX90614 Testing..");
  if (!mlx.begin()) {
    Serial.println("FAILED");
    while(true);
  } else {
    Serial.println("SUCCESS");
  }

  nodemcu.begin(14400);
  Serial.println("Program started");
}

const size_t CAPACITY = JSON_OBJECT_SIZE(16);
StaticJsonDocument<CAPACITY> doc;
JsonObject data = doc.to<JsonObject>();

void loop() {

  // Make sure to call update as fast as possible
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    data["ObjectTempInF"] = mlx.readObjectTempF();
    data["AmbientTempInC"] = mlx.readAmbientTempC();
    data["HeartRate"] = pox.getHeartRate();
    data["SpO2"] = pox.getSpO2();

    serializeJsonPretty(doc, Serial);
    // WriteBufferingStream bufferedWifiClient{nodemcu, CAPACITY};
    serializeJsonPretty(doc, nodemcu);
    // bufferedWifiClient.flush();
    
    Serial.println();
    doc.JsonDocument::clear();

    tsLastReport = millis();
  }
}