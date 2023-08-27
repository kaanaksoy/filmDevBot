#include "encoder_helper.hpp"

namespace SystemEncoder
{
    // Function to await user confirmation using the encoder
    EncoderInputType encoderAwaitConfirm()
    {
        EncoderInputType command = EncoderNone; // Initialize command as none
        while (command != EncoderEnter)         // Continue loop until Enter command is received
        {
            command = getCommand(command); // Get user's command using the encoder
            switch (command)
            {
            case EncoderEnter:
                return EncoderEnter; // Return Enter command if received
            case EncoderExit:
                return EncoderExit; // Return Exit command if received
            default:
                break; // Continue loop for other commands
            }

            // Blink LEDs if indicator state is busy
            if (State.ledState == IndicatorStateType::BUSY)
            {
                Indicators::blinkLEDs(); // Call function to blink LEDs
            }

            // Buzz the buzzer if indicator state is busy
            if (State.buzzerState == IndicatorStateType::BUSY)
            {
                Indicators::buzz();
            }
        }
    }

    // Function to initialize the encoder module
    void initEncoder()
    {
        // Set pins as inputs and enable pull-up resistors
        pinMode(ENC_CLK, INPUT);
        pinMode(ENC_DT, INPUT);
        pinMode(ENC_SW, INPUT_PULLUP);
    }

    // Function to get the user's command for menu navigation using the encoder
    EncoderInputType getCommand(EncoderInputType prevCommand)
    {
        static bool needDebounce; // Flag for debouncing
        // Check if encoder button press is allowed based on state
        if (!State.checkEncoder)
        {
            if (needDebounce)
            {
                delay(100); // Debounce delay
                needDebounce = false;
            }

            // Check if encoder button is pressed and previous command was none
            if (digitalRead(ENC_SW) == LOW && prevCommand == EncoderNone)
            {
                unsigned long timeStamp = millis(); // Get current time
                while (digitalRead(ENC_SW) == LOW)
                {
                    // Check for long press
                    if (millis() > timeStamp + LONG_PRESS_DUR)
                    {
                        tone(BUZZER_PIN, 4000, 125); // Buzz the buzzer
                        needDebounce = true;
                        return EncoderExit; // Return Exit command for long press
                    }
                }
                tone(BUZZER_PIN, 4000, 125); // Buzz the buzzer
                return EncoderEnter;         // Return Enter command for short press
            }
        }

        // Logic for left-right movement of the encoder
        static uint8_t lrmem = 3;
        static int lrsum = 0;
        static int8_t TRANS[] = {0, -1, 1, 14, 1, 0, 14, -1, -1, 14, 0, 1, 14, 1, -1, 0};

        int8_t l = digitalRead(ENC_CLK);
        int8_t r = digitalRead(ENC_DT);

        // Move previous value 2 bits to the left and add in our new values
        lrmem = ((lrmem & 0x03) << 2) + 2 * l + r;

        // Convert the bit pattern to a movement indicator (14 = impossible, ie switch bounce)
        lrsum += TRANS[lrmem];

        // Return the appropriate encoder command based on the movement
        /* encoder not in the neutral (detent) state */
        if (lrsum % 4 != 0)
        {
            return EncoderNone;
        }

        /* encoder in the neutral state - clockwise rotation*/
        if (lrsum == 4)
        {
            lrsum = 0;
            return EncoderRight;
        }

        /* encoder in the neutral state - anti-clockwise rotation*/
        if (lrsum == -4)
        {
            lrsum = 0;
            return EncoderLeft;
        }

        // An impossible rotation has been detected - ignore the movement
        lrsum = 0;
        return EncoderNone;
    }
} // namespace Encoder
