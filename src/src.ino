#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Conf.h>
#include <SInteg.h>
#include <LDR_SInteg.h>
#include <ReedSwitch_SInteg.h>
#include <DHT_SInteg.h>

WiFiClient wclient;
SInteg _core(wclient);

// Initialize Reed Switch sensor
ReedSwitch_SInteg reedswitch(REEDSWITCHPIN, _core);
// Initialize LDR sensor
LDR_SInteg ldr(LDRPIN, _core);
// Initialize DHT sensor
DHT_SInteg dht(DHTPIN, DHTTYPE, _core);

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("______SInteg______");

  // Set the reed switch pin as input pullup
  pinMode(REEDSWITCHPIN, INPUT_PULLUP);
  // Set the built in led pin as output mode
  pinMode(LED_BUILTIN, OUTPUT);
  // Turn off built in led
  digitalWrite(LED_BUILTIN, HIGH);

  // Set basic WiFi information and connects to the passed network
  _core.setup_wifi(WIFI_SSID, WIFI_PASSWD);

  // Blink the built in led 5 times to indicate wifi is connected
  blink(5);

  // Set basic required information for PubSubClient
  _core.setup_mqtt(MQTT_USERNAME, MQTT_PASSWD, MQTT_SERVER, MQTT_PORT);
}

void loop() {
  // Check if the connection with MQTT server is still alive
  if (! _core.isMQTTConnected()) {
    // Reconnect to the MQTT server;
    _core.reconnect_mqtt();
  }

  // Allow the client to process incoming messages (if so) and maintain its
  // connection to the server
  _core.MQTTLoop();

  // 2 seconds delay before the next sensor read
  delay(2000);

  // Read LDR state from LDR pin and publish the value
  if (! ldr.publish()) {
    Serial.println("Failed to publish LDR");
  }
  // Read reed switch state from reed switch pin and publish the value
  if (! reedswitch.publish()) {
    Serial.println("Failed to publish Reed Switch");
  }
  // Read temperature from dht sensor and publish the value
  if (! dht.publishTemperature()) {
    Serial.println("Failed to publish Temperature");
  }
  // Read humidity from dht sensor and publish the value
  if (! dht.publishHumidity()) {
    Serial.println("Failed to publish Humidity");
  }
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
