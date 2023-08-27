#ifndef LED_HELPER_HPP
// Define the header guard symbol to prevent multiple inclusion
#define LED_HELPER_HPP

#include <Arduino.h>
#include <FastLED.h> // Include the FastLED library for controlling LEDs

#include "../../globals.h"    // Include global configuration/settings
#include "../../debugUtils.h" // Include debugging utilities if available

namespace Indicators
{
    // Function to initialize LEDs
    void initLEDs();

    // Function to toggle or perform an action on temporary LEDs
    void tempLEDs(IndicatorParamType action = IndicatorParamType::TOGGLE);

    // Function to display LEDs relatively based on a value and target
    void relativeLED(int val, int target);

    // Function to blink LEDs with a specified color and interval
    void blinkLEDs(CRGB::HTMLColorCode color = CRGB::Red,
                   int interval = BLINK_NORMAL_INTERVAL,
                   IndicatorParamType action = IndicatorParamType::TOGGLE);

    // Function to calculate the number of LEDs to light up per step in a progress bar
    int calculateProgressBarStep(int maxSteps);

    // Function to display LEDs as a progress bar with a specified progress value
    void progressBarLEDs(uint8_t progress = 0);

    /*
--- buzz | UI Helper Functions ---
Simple buzzer control, used to simplify code.
*/
    // Function to control the buzzer with a specified interval and action
    void buzz(int interval = BUZZER_NORMAL_INTERVAL, IndicatorParamType action = IndicatorParamType::TOGGLE);
} // namespace Indicators

#endif