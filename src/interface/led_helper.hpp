#ifndef LED_HELPER_HPP
#define LED_HELPER_HPP

#include <Arduino.h>

#include "../../globals.h"
#include "../utilities/state_manager.hpp"

#define NUM_LEDS 1
#define RGB_LED_PIN 12
#define STATUS_LED_BRIGHTNESS 50 // (0-255) Sets max brightness
#define INTERVAL 1000

namespace StatusLED
{
    void blink(int count = 0);
    
} // namespace StatusLED


#endif