//ThatsEngineering
//Sending Data from Arduino to NodeMCU Via Serial Communication
//NodeMCU code

//Include Lib for Arduino to Nodemcu
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
// #include <SimpleTimer.h>

// #define BLYNK_PRINT Serial

//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);

// char auth[] = "e8edf95085954103b0c8c6b2ff4c9745";

// // Your WiFi credentials.
// // Set password to "" for open networks.
// char ssid[] = "ZONG MBB-E8231-6E63";
// char pass[] = "08659650";
// SimpleTimer timer;

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  nodemcu.begin(14400);
  while (!Serial) continue;
  // Blynk.begin(auth, ssid, pass);

  // timer.setInterval(1000L,sensorvalue1);
  // timer.setInterval(1000L,sensorvalue2);
  // timer.setInterval(1000L,sensorvalue3);
  // timer.setInterval(1000L,sensorvalue4);
}

// void sensorvalue1()
// {
// int sdata = firstVal; // humidity value
//   // You can send any value at any time.
//   // Please don't send more that 10 values per second.
//   Blynk.virtualWrite(V2, sdata);

// }
// void sensorvalue2()
// {
// int sdata = secondVal; // temperature value
//   if (sdata > 25 )
//   {
//     Blynk.notify("temperature exceeded!!!");
//   }

//   // You can send any value at any time.
//   // Please don't send more that 10 values per second.
//   Blynk.virtualWrite(V3, sdata);

// }


const size_t CAPACITY = JSON_OBJECT_SIZE(16);
StaticJsonDocument<CAPACITY> doc;

JsonObject data = doc.as<JsonObject>();

void loop() {

  // StaticJsonBuffer<1000> jsonBuffer;
  // JsonObject& data = jsonBuffer.parseObject(nodemcu);
  // deserialize the object
  if (nodemcu.available() > 0) {
    deserializeJson(doc, nodemcu);
    data = doc.as<JsonObject>();

    float AmbientTempC = data["AmbientTempInC"];
    float ObjectTempF = data["ObjectTempInF"];
    float HeartRate = data["HeartRate"];
    uint8_t SpO2 = data["SpO2"];
    if(round(AmbientTempC) != 0) {
      Serial.print("Ambient Temperature in Celsius:  ");      
      Serial.println(AmbientTempC);
      Serial.print("Object Temperature in Farh: ");
      Serial.println(ObjectTempF);
      Serial.print("Heart Rate:  ");
      Serial.println(HeartRate);
      Serial.print("SpO2:  ");
      Serial.println(SpO2);
      Serial.println("-----------------------------------------");
      // serializeJson(doc, Serial);
      // Serial.println();
    }
  }
  // extract the data
  // float ObjectTempF = data["ObjectTempInF"];
  // float AmbientTempC = data["AmbientTempInC"];
  // if (data == JsonObject::invalid()) {
  //   Serial.println("Invalid Json Object");
  //   jsonBuffer.clear();
  //   return;
  // }
  // Serial.println("JSON Object Recieved");

  // Serial.print("Heart Rate:  ");
  // float HeartRate = data["HeartRate"];
  // Serial.println(HeartRate);

  // Serial.print(" SpO2 level:  ");
  // uint8_t SpO2 = data["SpO2"];
  // Serial.println(SpO2);


  // Serial.print(" Object Temperature in Farh: ");
  // float ObjectTempF = data["ObjectTempInF"];
  // Serial.println(ObjectTempF);

  // Serial.println("-----------------------------------------");
  doc.JsonDocument::clear();
}
