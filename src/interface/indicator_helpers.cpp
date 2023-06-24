#include "indicator_helpers.hpp"

namespace Indicators
{
    CRGB gLEDS[NUM_LEDS]; // Create an array to hold the LED colors.
    DEFINE_GRADIENT_PALETTE(temp_gp){
        0, 0, 0, 255,        // Blue
        NUM_LEDS, 255, 0, 0, // Red
        255, 255, 0, 0,      // RED
    };

    CRGBPalette16 tempPal = temp_gp;

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

    void tempLEDs(IndicatorParamType action)
    {
        switch (action)
        {
        case IndicatorParamType::START:
            State.ledState = IndicatorStateType::BUSYAuto;
            for (int i = 0; i < NUM_LEDS; i++)
            {
                gLEDS[i] = ColorFromPalette(tempPal, i);
                gLEDS[(NUM_LEDS / 2) - 1].setRGB(0, 0, 0);
                gLEDS[(NUM_LEDS / 2)].setRGB(0, 255, 0);
                gLEDS[(NUM_LEDS / 2) + 1].setRGB(0, 0, 0);
                FastLED.show();
                delay(17);
            }
            break;
        case IndicatorParamType::STOP:
            fill_solid(gLEDS, NUM_LEDS, CRGB::Black);
            FastLED.show();
            State.ledState = IndicatorStateType::AVAILABLE;
            break;
        case IndicatorParamType::TOGGLE:
            for (int i = 0; i < NUM_LEDS; i++)
            {
                gLEDS[i] = ColorFromPalette(tempPal, i);
                gLEDS[(NUM_LEDS / 2) - 1].setRGB(0, 0, 0);
                gLEDS[(NUM_LEDS / 2)].setRGB(0, 255, 0);
                gLEDS[(NUM_LEDS / 2) + 1].setRGB(0, 0, 0);
            }
        default:
            break;
        }

        return;
    }

    void relativeLED(int val, int target)
    {
        int centerIndex = (NUM_LEDS / 2) + ((val - target) / 100);

        if (centerIndex > NUM_LEDS - 1)
        {
            centerIndex = NUM_LEDS - 1;
        }
        else if (centerIndex < 0)
        {
            centerIndex = 0;
        }

        tempLEDs();
        gLEDS[centerIndex - 1].setRGB(0, 0, 0);
        gLEDS[centerIndex].setRGB(0, 255, 0);
        gLEDS[centerIndex + 1].setRGB(0, 0, 0);

        FastLED.show();
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
    // Calculates how many leds to be lit per step based on num leds available
    int calculateProgressBarStep(int maxSteps)
    {
        return NUM_LEDS / maxSteps;
    }

    void progressBarLEDs(uint8_t progress)
    {
        static bool progressBarRunning;

        switch (progress == 0) // If func was called without a parameter.

        {
        case true:
            if (progressBarRunning)
            { // If there is an instance of the progressbar running, we turn it off.
                DEBUG_PRINT("ProgressBarFinished");
                fill_solid(gLEDS, NUM_LEDS, CRGB::Black);
                progressBarRunning = false;
                State.ledState = IndicatorStateType::AVAILABLE;
            }
            else // Else start the progress bar.
            {
                DEBUG_PRINT("ProgressBarStarted");
                State.ledState = IndicatorStateType::BUSYAuto;
                progressBarRunning = true;
                fill_solid(gLEDS, NUM_LEDS, DIM_WHITE);
            }
            break;
        case false:
            if (!progressBarRunning)
            {
                DEBUG_PRINT("ProgressBarStarted");
                State.ledState = IndicatorStateType::BUSYAuto;
                progressBarRunning = true;
                fill_solid(gLEDS, NUM_LEDS, DIM_WHITE);
            }
            DEBUG_PRINT("ProgressBarUpdated: ");
            DEBUG_PRINT(progress);
            if (progress == 100)
            {
                fill_solid(gLEDS, NUM_LEDS, CRGB::Green);
            }
            else
            {
                fill_solid(gLEDS, progress, CRGB::Green);
            }

            // int ledsToFill = (progress * NUM_LEDS / 100) + 2; // Calculate the number of leds to fill per step.
            // fill_solid(gLEDS, progress, CRGB::Green);
        default:
            break;
        }
        FastLED.show();
        return;
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
