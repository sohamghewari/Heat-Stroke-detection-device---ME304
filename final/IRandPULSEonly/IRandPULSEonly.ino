#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//Arduino to NodeMCU Lib
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(5, 6);

float HeartRate;
float SpO2;++
float AmbientTempC;
float ObjectTempF;

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected() {
  // Serial.println("");
}

void setup() {
  Serial.begin(115200);

  Serial.print("Initializing pulse oximeter..");

  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;)
      ;
  } else {
    Serial.println("SUCCESS");
  }

  // The default current for the IR LED is 50mA and it could be changed
  //   by uncommenting the following line. Check MAX30100_Registers.h for all the
  //   available options.
  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);

  Serial.println("Arduino MLX90614 Testing");
  mlx.begin();

  // nodemcu.begin(115200);
  // delay(1000);

  // Serial.println("Program started");
}

void loop() {


  // Make sure to call update as fast as possible
  pox.update();

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

    // StaticJsonBuffer<1000> jsonBuffer;
    // JsonObject& data = jsonBuffer.createObject();

    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");

    Serial.print("Ambient = ");
    Serial.print(mlx.readAmbientTempC());

    Serial.print("*C\tObject = ");
    Serial.print(mlx.readObjectTempC());
    Serial.println("*C");

    Serial.print("Ambient = ");
    Serial.print(mlx.readAmbientTempF());

    Serial.print("*F\tObject = ");
    Serial.print(mlx.readObjectTempF());
    Serial.println("*F");

    // //pulse sensor code
    // pox.update();
    //Serial.print("Heart rate:");
    //Serial.print(pox.getHeartRate());
    //  Serial.print("bpm / SpO2:");
    //       Serial.print(pox.getSpO2());
    //       Serial.println("%");


    Serial.println();
    tsLastReport = millis();

    // data["ObjectTempInF"] = ObjectTempF;
    // data["AmbientTempInC"] = AmbientTempC;
    // data["HeartRate"] = HeartRate;
    // data["SpO2"] = SpO2;

    // //Send data to NodeMCU
    // data.printTo(nodemcu);
    // jsonBuffer.clear();
  }
}