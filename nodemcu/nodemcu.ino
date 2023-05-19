#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ArduinoJson.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "";
char pass[] = "";
const int BUFFER_SIZE = 50;
char buf[BUFFER_SIZE];
int previousHumidity = 5;
int previousTemperature = 5;
int previousLight = 5;
int previousMoisture = 5;

BLYNK_WRITE(V4){
  int pin = param.asInt();
  digitalWrite(D2, pin);
}
BLYNK_WRITE(V5){
  int pin = param.asInt();
  digitalWrite(D2, pin);
}
BLYNK_WRITE(V6){
  int pin = param.asInt();
  digitalWrite(D3, pin);
}
void setup() {
  Serial.begin(9600);
  pinMode(D2, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  Blynk.begin(auth, ssid, pass);
}
void loop(){
  Blynk.run();
  if (Serial.available() > 0) {
    StaticJsonDocument<200> jsonDocs;
    deserializeJson(jsonDocs, Serial);

    int humidityData = jsonDocs["humidity"];
    int temperatureData = jsonDocs["temperature"];
    int newLightData = jsonDocs["new_light"];
    int moistureData = jsonDocs["moisture"];


    if (humidityData != previousHumidity && humidityData > 0) {
      Serial.println(humidityData);
      Blynk.virtualWrite(V0, humidityData);
      previousHumidity = humidityData;
    }

    if (temperatureData != previousTemperature && temperatureData > 0) {
      Serial.println(temperatureData);
      Blynk.virtualWrite(V1, temperatureData);
      previousTemperature = temperatureData;
      if(temperatureData < 50){
        // Blynk.virtualWrite(V4, HIGH);
        digitalWrite(D1, LOW);
      }
      else{
        // Blynk.virtualWrite(V4, LOW);
        digitalWrite(D1, HIGH);
      }
    }

    if (newLightData != previousLight && newLightData > 0) {
      Serial.println(newLightData);
      Blynk.virtualWrite(V2, newLightData);
      previousLight = newLightData;
      if(newLightData < 8){
        // Blynk.virtualWrite(V5, HIGH);
        digitalWrite(D2, HIGH);
      }
      else{
        // Blynk.virtualWrite(V5, LOW);
        digitalWrite(D2, LOW);
      }
    }

    if (moistureData != previousMoisture && moistureData > 0) {
      Serial.println(moistureData);
      Blynk.virtualWrite(V3, moistureData);
      previousMoisture = moistureData;
      if(moistureData < 50){
        // Blynk.virtualWrite(V6, HIGH);
        digitalWrite(D3, LOW);
      }
      else{
        // Blynk.virtualWrite(V6, LOW);
        digitalWrite(D3, HIGH);
      }
    }
  }
}