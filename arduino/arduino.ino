#include <dht.h>
#include <ArduinoJson.h>
#include <BH1750.h>
#include <Wire.h>
#include <i2cdetect.h>

// _____TEMPERATURE & HUMIDITY____
#define dht_apin A1
dht DHT;
int dht_output = 1;

// _____SOIL SENSOR____
int sensor_pin = A0;
int output_value;

// ______LIGHT PIN____
BH1750 lightMeter;
int light_val;
int light_op = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // ______LIGHT____
  Wire.begin();
  i2cdetect();
  lightMeter.begin();
  pinMode(5, OUTPUT);
}

void loop() {
  StaticJsonDocument<200> jsonDocs;

  // ______LIGHT____
  uint16_t lux = lightMeter.readLightLevel();
  light_val = map(lux, 20, 500 , 10, 100);
  Serial.println(light_val);
  
  // ______SOIL____
  output_value= analogRead(sensor_pin);
  output_value = map(output_value,1024,350,10,95);

  // ___DHT____
  DHT.read11(dht_apin);

  //____DATAS TRANSFER____
  jsonDocs["humidity"] = DHT.humidity;
  jsonDocs["temperature"] = DHT.temperature;
  jsonDocs["new_light"] = light_val;
  jsonDocs["moisture"] = output_value;

  serializeJson(jsonDocs, Serial);
  Serial.println("\n");
}