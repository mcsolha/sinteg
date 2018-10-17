#include <SInteg.h>

#ifndef LDR_SINTEG_H
#define LDR_SINTEG_H

class LDR_SInteg
{
    private:
        const uint8_t pin;
        SInteg& core;

    public:
        LDR_SInteg (uint8_t _pin, SInteg _core) : pin(_pin), core(_core) { };
        bool read ();
        bool publish ();
};

#endif
