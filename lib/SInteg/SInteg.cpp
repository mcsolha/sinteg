#include <ESP8266WiFi.h>
#include <SInteg.h>

void SInteg::setDebug (bool _debug)
{
    debug = _debug;
}

SInteg::SInteg (WiFiClient& _wclient)
{
    debug = false;
    base_topic = "sinteg/";
    client.setClient(_wclient);
}

void SInteg::setup_wifi (
    const char* _wifi_ssid,
    const char* _wifi_passwd
)
{
    wifi_ssid = _wifi_ssid;
    wifi_passwd = _wifi_passwd;

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(wifi_ssid);

    WiFi.begin(wifi_ssid, wifi_passwd);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("WiFi connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void SInteg::setup_mqtt (
    const char* _mqtt_username,
    const char* _mqtt_password,
    const char* _mqtt_server,
    uint16_t _mqtt_port
)
{
    mqtt_username = _mqtt_username;
    mqtt_password = _mqtt_password;
    mqtt_server = _mqtt_server;
    mqtt_port = _mqtt_port;

    if (debug) {
        Serial.println("");
        Serial.print("Setting mqtt - user: ");
        Serial.print(mqtt_username);
        Serial.print(", passwd: ");
        Serial.println(mqtt_password);
        Serial.print("server: ");
        Serial.print(mqtt_server);
        Serial.print(", port: ");
        Serial.println(mqtt_port);
    }

    client.setServer(mqtt_server, mqtt_port);
    Serial.println("MQTT set!");
}

void SInteg::reconnect_mqtt ()
{
    int tries = 5;
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.println("");
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("SInteg", mqtt_username, mqtt_password)) {
            Serial.println("SInteg connected to server");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            if (--tries == 0) {
                Serial.println("Maximum tries reached, resetting SInteg");
                ESP.restart();
            }
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

boolean SInteg::isMQTTConnected ()
{
    return client.connected();
}

boolean SInteg::MQTTLoop ()
{
    return client.loop();
}

boolean SInteg::MQTTPublish (const char* topic, const char* payload)
{
    return client.publish(topic, payload);
}
