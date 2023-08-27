#include "indicator_helpers.hpp"
#define ANIMATION_DELAY 17
namespace Indicators
{
    // Create an array to hold the LED colors.
    CRGB gLEDS[NUM_LEDS];
    // Create a gradient palette for temporary LEDs
    DEFINE_GRADIENT_PALETTE(temp_gp){
        0, 0, 0, 255,        // Blue
        NUM_LEDS, 255, 0, 0, // Red
        255, 255, 0, 0,      // RED
    };

    // Define a color palette using the gradient palette
    CRGBPalette16 tempPal = temp_gp;

    // Initialize LEDs with a rainbow effect
    void initLEDs()
    {
        // Create an LED object with specified parameters
        FastLED.addLeds<WS2812B, LED_PIN, GRB>(gLEDS, NUM_LEDS);
        FastLED.setBrightness(LED_BRIGHTNESS);

        // Apply a rainbow effect to LEDs
        for (int i = 0; i < NUM_LEDS; i++)
        {
            gLEDS[i] = CHSV(i * 5, 255, 255);
            FastLED.show();         // Display LED colors
            delay(ANIMATION_DELAY); // Delay for animation effect
        }
        delay(ANIMATION_DELAY);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            gLEDS[i] = CRGB::Black; // Turn off LEDs
            FastLED.show();
            delay(ANIMATION_DELAY);
        }
    }

    // Control temporary LEDs with specified action
    void tempLEDs(IndicatorParamType action)
    {
        switch (action)
        {
        case IndicatorParamType::START:
            State.ledState = IndicatorStateType::BUSYAuto;
            // Set LED colors with temporary palette and customize middle LEDs
            for (int i = 0; i < NUM_LEDS; i++)
            {
                gLEDS[i] = ColorFromPalette(tempPal, i);
                gLEDS[(NUM_LEDS / 2) - 1].setRGB(0, 0, 0);
                gLEDS[(NUM_LEDS / 2)].setRGB(0, 255, 0);
                gLEDS[(NUM_LEDS / 2) + 1].setRGB(0, 0, 0);
                FastLED.show();
                delay(ANIMATION_DELAY);
            }
            break;
        case IndicatorParamType::STOP:
            // Turn off all LEDs
            fill_solid(gLEDS, NUM_LEDS, CRGB::Black);
            FastLED.show();
            State.ledState = IndicatorStateType::AVAILABLE;
            break;
        case IndicatorParamType::TOGGLE:
            // Set LED colors with temporary palette and customize middle LEDs
            for (int i = 0; i < NUM_LEDS; i++)
            {
                gLEDS[i] = ColorFromPalette(tempPal, i);
                gLEDS[(NUM_LEDS / 2) - 1].setRGB(0, 0, 0);
                gLEDS[(NUM_LEDS / 2)].setRGB(0, 255, 0);
                gLEDS[(NUM_LEDS / 2) + 1].setRGB(0, 0, 0);
            }
            // Fall-through intended
        default:
            break;
        }

        return;
    }

    // Display LEDs relative to a value and target
    void relativeLED(int val, int target)
    {
        int centerIndex = (NUM_LEDS / 2) + ((val - target) / 100);

        // Ensure the centerIndex stays within valid bounds
        if (centerIndex > NUM_LEDS - 1)
        {
            centerIndex = NUM_LEDS - 1;
        }
        else if (centerIndex < 0)
        {
            centerIndex = 0;
        }

        // Display temporary LEDs with custom center LEDs
        tempLEDs();
        gLEDS[centerIndex - 1].setRGB(0, 0, 0);
        gLEDS[centerIndex].setRGB(0, 255, 0);
        gLEDS[centerIndex + 1].setRGB(0, 0, 0);

        FastLED.show();
    }

    // Blink LEDs with specified color, interval, and action
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
            // Toggle LEDs based on blink interval
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
            // Turn off LEDs and update state
            ledOn = false;
            fill_solid(gLEDS, NUM_LEDS, CRGB::Black);
            FastLED.show();
            State.ledState = IndicatorStateType::AVAILABLE;
        default:
            break;
        }
        return;
    }
    // Calculate the number of LEDs to light up per step in a progress bar
    int calculateProgressBarStep(int maxSteps)
    {
        return NUM_LEDS / maxSteps;
    }

    // Display LEDs as a progress bar with a specified progress value
    void progressBarLEDs(uint8_t progress)
    {
        static bool progressBarRunning;

        switch (progress == 0) // func was called without a parameter.

        {
        case true:
            if (progressBarRunning)
            {
                // Turn off progress bar and update state
                DEBUG_PRINT("ProgressBarFinished");
                fill_solid(gLEDS, NUM_LEDS, CRGB::Black);
                progressBarRunning = false;
                State.ledState = IndicatorStateType::AVAILABLE;
            }
            else
            {
                // Start progress bar and update state
                DEBUG_PRINT("ProgressBarStarted");
                State.ledState = IndicatorStateType::BUSYAuto;
                progressBarRunning = true;
                fill_solid(gLEDS, NUM_LEDS, DIM_WHITE);
            }
            break;
        case false:
            if (!progressBarRunning)
            {
                // Start progress bar and update state
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
    // Control the buzzer with specified interval and action
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
            // Toggle buzzer based on buzz interval
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
            // Turn off buzzer and update state
            buzzerOn = false;
            noTone(BUZZER_PIN);
            State.buzzerState = IndicatorStateType::AVAILABLE;
            break;
        default:
            break;
        }
    }
} // namespace Indicators
