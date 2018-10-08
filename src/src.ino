// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include <DHT.h>
#include <DHT_U.h>
#include "base.h"

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(REEDSWITCHPIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RECONNECTPIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.println("#####TCC!#####");

  WiFiManager wifiManager;

  wifiManager.autoConnect(AP_SSID);

  Serial.print("Connected to: ");
  Serial.println(WiFi.SSID());
  Serial.println();
  blink(5);

  dht.begin();
}

void loop() {
  int reconnect = digitalRead(RECONNECTPIN);
  // Wait a few seconds between measurements.
  if (reconnect || WiFi.status() != WL_CONNECTED) {
    blink(3);
    WiFiManager wifiManager;
    wifiManager.startConfigPortal(AP_SSID);
    Serial.print("Connected to: ");
    Serial.println(WiFi.SSID());
    blink(5);
  }
  delay(2000);
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

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

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
