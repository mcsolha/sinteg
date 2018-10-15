// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <WiFiManager.h>  
#include <DHT.h>
#include <DHT_U.h>
#include "base.h"

WiFiClient wclient;
PubSubClient client(wclient);
// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", "username", "password")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

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

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);

  dht.begin();
}

void loop() {
  int hasToReconnect = digitalRead(RECONNECTPIN);
  // Wait a few seconds between measurements.
  if (hasToReconnect || WiFi.status() != WL_CONNECTED) {
    client.disconnect();
    blink(3);
    WiFiManager wifiManager;
    wifiManager.startConfigPortal(AP_SSID);
    Serial.print("Connected to: ");
    Serial.println(WiFi.SSID());
    blink(5);
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  client.publish("outTopic", "PUBLICOU");

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
