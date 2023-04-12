#ifndef LED_HELPER_HPP
#define LED_HELPER_HPP

#include <Arduino.h>

#include "../../globals.h"

namespace StatusLED
{
    void blink(int count = 0);

} // namespace StatusLED

#endif