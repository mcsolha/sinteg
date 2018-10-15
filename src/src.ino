// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <DHT_U.h>
#include <sinteg.h>
#include <ArduinoJson.h>
#include "base.h"

WiFiClient wclient;
Sinteg _core(wclient);

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  pinMode(REEDSWITCHPIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.println("#####TCC!#####");

  _core.setup_wifi(WIFI_SSID, WIFI_PASSWD);
  Serial.println(WiFi.SSID());
  Serial.println();
  blink(5);
  _core.setup_mqtt(MQTT_USERNAME, MQTT_PASSWD, MQTT_SERVER, MQTT_PORT);

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected...");
    blink(3);
    ESP.reset();
  }

  if (!_core.isMQTTConnected()) {
    _core.reconnect_mqtt();
  }
  _core.MQTTLoop();

  delay(2000);

  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& tempEncoder = JSONbuffer.createObject();

  int ldr_state = digitalRead(LDRPIN);
  int reed_state = digitalRead(REEDSWITCHPIN);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  
  tempEncoder["celsius"] = hic;
  tempEncoder["fahrenheit"] = hif;

  char tempMsgBuffer[100];
  tempEncoder.printTo(tempMsgBuffer, sizeof(tempMsgBuffer));

  Serial.println(tempMsgBuffer);
  if (_core.MQTTPublish("sinteg/temp", tempMsgBuffer)) {
    Serial.println("temp published!");
  } else {
    Serial.println("temp not pub :(");
  }
  // if (_core.MQTTPublish("sinteg/humid", ))
  // Serial.print("Humidity: ");
  // Serial.print(h);
  // Serial.print(" %\t");
  // Serial.print("Temperature: ");
  // Serial.print(t);
  // Serial.print(" *C ");
  // Serial.print(f);
  // Serial.print(" *F\t");
  // Serial.print("Heat index: ");
  // Serial.print(hic);
  // Serial.print(" *C ");
  // Serial.print(hif);
  // Serial.println(" *F");

  Serial.print("LDR:  ");
  Serial.println(ldr_state);

  Serial.print("REED SWITCH:  ");
  Serial.println(reed_state);
}

void blink(int times) {
  uint8_t i = 0;

  for(i=0; i < times; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
  }
}
