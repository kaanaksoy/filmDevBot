#include "indicator_helpers.hpp"

namespace Indicators
{
    CRGB gLEDS[NUM_LEDS]; // Create an array to hold the LED colors.

    // In addition to creating the led object, execute a quick rainbow effect for style points.
    void initLEDs()
    {
        // Create an LED object.
        FastLED.addLeds<WS2812B, LED_PIN, GRB>(gLEDS, NUM_LEDS);
        FastLED.setBrightness(LED_BRIGHTNESS);

        for (int i = 0; i < NUM_LEDS; i++)
        {
            gLEDS[i] = CHSV(i * 5, 255, 255);
            FastLED.show();
            delay(17);
        }
        delay(17);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            gLEDS[i] = CRGB::Black;
            FastLED.show();
            delay(17);
        }
    }

    // Simple non-blocking blink function.
    void blinkLEDs(CRGB::HTMLColorCode color,
                   int interval,
                   IndicatorParamType action)
    {
        static int blinkInterval;
        static CRGB::HTMLColorCode colorCode;
        static bool ledOn = false;
        static unsigned long previousToggleMillis;

        unsigned long currentMillis = millis();

        switch (action)
        {
        case IndicatorParamType::START:
            State.ledState = IndicatorStateType::BUSY;
            ledOn = true;
            blinkInterval = interval;
            colorCode = color;
            previousToggleMillis = currentMillis;
            fill_solid(gLEDS, NUM_LEDS, colorCode);
            FastLED.show();
            break;
        case IndicatorParamType::TOGGLE:
            if (currentMillis - previousToggleMillis >= blinkInterval)
            {
                previousToggleMillis = currentMillis;
                ledOn = !ledOn;
                if (ledOn)
                {
                    fill_solid(gLEDS, NUM_LEDS, colorCode);
                }
                else
                {
                    fill_solid(gLEDS, NUM_LEDS, CRGB::Black);
                }
                FastLED.show();
            }
            break;
        case IndicatorParamType::STOP:
            ledOn = false;
            fill_solid(gLEDS, NUM_LEDS, CRGB::Black);
            FastLED.show();
            State.ledState = IndicatorStateType::AVAILABLE;
        default:
            break;
        }
        return;
    }

    void progressBarLEDs(uint8_t progress)
    {
        static bool progressBarRunning;

        switch (progress = 0) // If func was called without a parameter.

        {
        case true:
            if (progressBarRunning)
            { // If there is an instance of the progressbar running, we turn it off.
                fill_solid(gLEDS, NUM_LEDS, CRGB::Black);
                progressBarRunning = false;
                State.ledState = IndicatorStateType::AVAILABLE;
            }
            else // Else start the progress bar.
            {
                State.ledState = IndicatorStateType::BUSY;
                progressBarRunning = true;
                fill_solid(gLEDS, NUM_LEDS, DIM_WHITE);
            }
            break;
        case false:
            int ledsToFill = (progress * NUM_LEDS / 100) + 2; // Calculate the number of leds to fill per step.
            fill_solid(gLEDS, ledsToFill, CRGB::Green);
        default:
            break;
        }
        FastLED.show();
    }

    /*
--- buzz | UI Helper Functions ---
Simple buzzer control, used to simplify code.
*/
    void buzz(int interval, IndicatorParamType action)
    {
        static int buzzInterval;
        static bool buzzerOn = false;
        static unsigned long previousToggleMillis;

        unsigned long currentMillis = millis();

        switch (action)
        {
        case IndicatorParamType::START:
            State.buzzerState = IndicatorStateType::BUSY;
            buzzerOn = true;
            buzzInterval = interval;
            previousToggleMillis = currentMillis;
            tone(BUZZER_PIN, 4000);
            break;
        case IndicatorParamType::TOGGLE:
            if (currentMillis - previousToggleMillis >= buzzInterval)
            {
                previousToggleMillis = currentMillis;
                buzzerOn = !buzzerOn;
                if (buzzerOn)
                {
                    tone(BUZZER_PIN, 4000);
                }
                else
                {
                    noTone(BUZZER_PIN);
                }
            }
            break;
        case IndicatorParamType::STOP:
            buzzerOn = false;
            noTone(BUZZER_PIN);
            State.buzzerState = IndicatorStateType::AVAILABLE;
            break;
        default:
            break;
        }
    }
} // namespace Indicators
