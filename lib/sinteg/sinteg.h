#include <PubSubClient.h>
#include <WiFiClient.h>

#ifndef SINTEG_H
#define SINTEG_H

class Sinteg {
    private:
        PubSubClient client;
        const char* mqtt_username;
        const char* mqtt_password;
        const char* mqtt_server;
        uint16_t    mqtt_port;
        const char* wifi_ssid;
        const char* wifi_passwd;
        bool debug;

    public:
        Sinteg (WiFiClient _wclient);
        void setup_mqtt (
            const char* _mqtt_username,
            const char* _mqtt_password,
            const char* _mqtt_server,
            uint16_t _mqtt_port
        );
        void setup_wifi (
            const char* _wifi_ssid,
            const char* _wifi_passwd
        );
        void reconnect_mqtt ();
        int isMQTTConnected ();
        boolean MQTTLoop ();
        boolean MQTTPublish (const char* topic, const char* payload);
        void setDebug (bool _debug);
};

#endif
