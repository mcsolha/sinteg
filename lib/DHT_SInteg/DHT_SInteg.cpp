#include <DHT_SInteg.h>
#include <ArduinoJson.h>

DHT_SInteg::DHT_SInteg (uint8_t pin, uint8_t type, SInteg _core)
    : dht(pin, type)
    , core(_core)
{
    dht.begin();
}

float DHT_SInteg::read (DHT_SIntegMode_t read_mode)
{
    float value = 0;

    switch (read_mode) {
        case HUMIDITY:
            value = dht.readHumidity();
            break;
        case TEMPERATURE_C:
            value = dht.readTemperature();
            break;
        case TEMPERATURE_F:
            value = dht.readTemperature(true);
            break;
    }

    if (isnan(value)) {
        Serial.println("Failed to read DHT sensor!");
    }
    
    return value;
}

float DHT_SInteg::getHeatIndex (bool isFahrenheit = false)
{
    float temperature = isFahrenheit
        ? read(TEMPERATURE_F)
        : read(TEMPERATURE_C);
    
    float humidity = read(HUMIDITY);

    if (!isnan(temperature) && !isnan(humidity)) {
        return dht.computeHeatIndex(temperature, humidity, isFahrenheit);
    }

    return NAN;
}

bool DHT_SInteg::publishTemperature ()
{
    float hiC = getHeatIndex();
    float hiF = getHeatIndex(true);
    char tempMsgBuffer[100];
    String topic = String(core.base_topic) + "temperature";

    Serial.print("Celsius: ");
    Serial.println(hiC);
    Serial.print("Fahren: ");
    Serial.println(hiF);

    if (isnan(hiC) || isnan(hiF)) {
        return false;
    }

    // JSON Buffer of 300 bytes to create an encoder
    StaticJsonBuffer<300> JSONbuffer;
    // Create an JSON Object to save temperature sensor data
    JsonObject& tempEncoder = JSONbuffer.createObject();


    tempEncoder["celsius"] = hiC;
    tempEncoder["fahrenheit"] = hiF;

    tempEncoder.printTo(tempMsgBuffer, sizeof(tempMsgBuffer));

    return core.MQTTPublish(topic.c_str(), tempMsgBuffer);
}

bool DHT_SInteg::publishHumidity ()
{
    String topic = String(core.base_topic) + "humidity";
    float humidity = read(HUMIDITY);

    if (isnan(humidity)) {
        return false;
    }

    return core.MQTTPublish(topic.c_str(), String(humidity).c_str());
}
