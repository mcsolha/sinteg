#include <LDR_SInteg.h>

bool LDR_SInteg::read () 
{
    return digitalRead(pin);
}

bool LDR_SInteg::publish ()
{
    String topic = String(core.base_topic) + "ldr";

    return core.MQTTPublish(
        topic.c_str(),
        ! read() ? "on" : "off"
    );
}
