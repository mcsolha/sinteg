#include <ReedSwitch_SInteg.h>

bool ReedSwitch_SInteg::read () 
{
    return digitalRead(pin);
}

bool ReedSwitch_SInteg::publish ()
{
    String topic = String(core.base_topic) + "reedswitch";

    return core.MQTTPublish(
        topic.c_str(),
        read() ? "open" : "close"
    );
}
