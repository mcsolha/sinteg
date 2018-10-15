#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "base.h"

class MQTT {
    public:
    WiFiClient client;
    Adafruit_MQTT_Client mqtt;
    WiFiManager wifiManager;
    MQTT () : mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD) {};
};
