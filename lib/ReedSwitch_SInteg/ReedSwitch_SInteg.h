#include <SInteg.h>

#ifndef REED_SWITCH_SINTEG_H
#define REED_SWITCH_SINTEG_H

class ReedSwitch_SInteg
{
    private:
        const uint8_t pin;
        SInteg& core;

    public:
        ReedSwitch_SInteg (uint8_t _pin, SInteg& _core) : pin(_pin), core(_core) { };
        bool read ();
        bool publish ();
};

#endif
