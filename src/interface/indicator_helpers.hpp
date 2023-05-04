#ifndef LED_HELPER_HPP
#define LED_HELPER_HPP

#include <Arduino.h>
#include <FastLED.h>

#include "../../globals.h"

namespace Indicators
{
    enum IndicatorParamType
    {
        START,
        STOP,
        TOGGLE
    };

    void initLEDs();
    void blinkLEDs(CRGB::HTMLColorCode color = CRGB::Red,
                   int interval = BLINK_NORMAL_INTERVAL,
                   IndicatorParamType action = IndicatorParamType::TOGGLE);

    void progressBarLEDs(uint8_t progress = 0);

    /*
--- buzz | UI Helper Functions ---
Simple buzzer control, used to simplify code.
*/
    void buzz(int interval = BUZZER_NORMAL_INTERVAL, IndicatorParamType action = IndicatorParamType::TOGGLE);
} // namespace Indicators

#endif