#include <DHT.h>
#include <DHT_U.h>
#include <SInteg.h>

#ifndef DHT_SInteg_H
#define DHT_SInteg_H

typedef enum DHT_SIntegMode
{
    HUMIDITY, TEMPERATURE_C, TEMPERATURE_F
} DHT_SIntegMode_t;

class DHT_SInteg
{
    private:
        DHT dht;
        SInteg& core;

    public:
        DHT_SInteg (uint8_t pin, uint8_t type, SInteg _core);
        float read (DHT_SIntegMode_t read_mode);
        float getHeatIndex (bool isFahrenheit);
        bool publishTemperature ();
        bool publishHumidity ();
};

#endif
