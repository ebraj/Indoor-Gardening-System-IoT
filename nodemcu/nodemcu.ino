#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLGmmGGs3i"
#define BLYNK_TEMPLATE_NAME "dht11nodemcu"
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
int systemSwitchVal = 1;
int fanPin = 0;
int lightPin = 0;
int pumpPin = 0;

BLYNK_WRITE(V4) {
  int pin = param.asInt();
  systemSwitchVal = pin;
}
BLYNK_WRITE(V5) {
  int pin = param.asInt();
  fanPin = pin;
}
BLYNK_WRITE(V6) {
  int pin = param.asInt();
  lightPin = pin;
}
BLYNK_WRITE(V7) {
  int pin = param.asInt();
  pumpPin = pin;
}

void setup() {
  Serial.begin(9600);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  Blynk.begin(auth, ssid, pass);
}
void loop() {
  Blynk.run();
  if (Serial.available() > 0) {
    StaticJsonDocument<200> jsonDocs;
    deserializeJson(jsonDocs, Serial);

    int humidityData = jsonDocs["humidity"];
    int temperatureData = jsonDocs["temperature"];
    int newLightData = jsonDocs["new_light"];
    int moistureData = jsonDocs["moisture"];
    if (humidityData != previousHumidity && humidityData > 0) {
      Blynk.virtualWrite(V0, humidityData);
      previousHumidity = humidityData;
    }

    if (temperatureData != previousTemperature && temperatureData > 0) {
      Blynk.virtualWrite(V1, temperatureData);
      previousTemperature = temperatureData;
    }

    if (newLightData != previousLight && newLightData > 0) {
      Blynk.virtualWrite(V2, newLightData);
      previousLight = newLightData;
    }

    if (moistureData != previousMoisture && moistureData > 0) {
      Blynk.virtualWrite(V3, moistureData);
      previousMoisture = moistureData;
    }

    if (systemSwitchVal == 0) {
      // Automatic Control
      Serial.println("I am inside automatic");
      if (temperatureData > 28) {
        digitalWrite(D1, LOW);
      } else {
        digitalWrite(D1, HIGH);
      }

      if (newLightData < 15) {
        digitalWrite(D2, HIGH);
      } else {
        digitalWrite(D2, LOW);
      }

      if (moistureData < 50) {
        digitalWrite(D3, LOW);
      } else {
        digitalWrite(D3, HIGH);
      }
    } else {
      Serial.println("I am inside manual");
      // Manual Control
      if (fanPin == 0) {
        digitalWrite(D1, HIGH);
        Serial.println("Fan running.");
        Blynk.logEvent("fan_running");
      } else {
        digitalWrite(D1, LOW);
        Serial.println("Fan stopped.");
        Blynk.logEvent("Fan stopped.");
      }
      if (lightPin == 0) {
        digitalWrite(D2, HIGH);
        Serial.println("Light running.");
        Blynk.logEvent("Light running.");
      } else {
        digitalWrite(D2, LOW);
        Serial.println("Light stopped.");
        Blynk.logEvent("Light stopped.");
      }
      if (pumpPin == 0) {
        digitalWrite(D3, HIGH);
        Serial.println("Pump running.");
        Blynk.logEvent("Pump running.");
      } else {
        digitalWrite(D3, LOW);
        Serial.println("Pump stopped.");
        Blynk.logEvent("Pump stopped.");
      }
    }
  }
}