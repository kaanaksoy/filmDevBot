#include "encoder_helper.hpp"

namespace SystemEncoder
{

    EncoderInputType encoderAwaitConfirm()
    {
        EncoderInputType command = EncoderNone;
        while (command != EncoderEnter)
        {
            command = getCommand(command);
            switch (command)
            {
            case EncoderEnter:
                return EncoderEnter;
            case EncoderExit:
                return EncoderExit;
            default:
                break;
            }
            if (State.ledState == IndicatorStateType::BUSY)
            {
                Indicators::blinkLEDs();
            }
            if (State.buzzerState == IndicatorStateType::BUSY)
            {
                Indicators::buzz();
            }
            
            
        }
    }

    void initEncoder()
    {
        // The encoder module has pullups built in.
        pinMode(ENC_CLK, INPUT);
        pinMode(ENC_DT, INPUT);
        pinMode(ENC_SW, INPUT_PULLUP);
    }

    EncoderInputType getCommand(EncoderInputType prevCommand)
    {
        static bool needDebounce;
        // Encoder button press
        if (!State.checkEncoder)
        {
            if (needDebounce)
            {
                delay(100);
                needDebounce = false;
            }

            if (digitalRead(ENC_SW) == LOW && prevCommand == EncoderNone)
            {
                unsigned long timeStamp = millis();
                while (digitalRead(ENC_SW) == LOW)
                {
                    if (millis() > timeStamp + LONG_PRESS_DUR)
                    {
                        tone(BUZZER_PIN, 4000, 125);
                        needDebounce = true;
                        return EncoderExit;
                    }
                }
                tone(BUZZER_PIN, 4000, 125);
                return EncoderEnter;
            }
        }

        // Left - Right movement of the encoder
        static uint8_t lrmem = 3;
        static int lrsum = 0;
        static int8_t TRANS[] = {0, -1, 1, 14, 1, 0, 14, -1, -1, 14, 0, 1, 14, 1, -1, 0};

        int8_t l = digitalRead(ENC_CLK);
        int8_t r = digitalRead(ENC_DT);

        // Move previous value 2 bits to the left and add in our new values
        lrmem = ((lrmem & 0x03) << 2) + 2 * l + r;

        // Convert the bit pattern to a movement indicator (14 = impossible, ie switch bounce)
        lrsum += TRANS[lrmem];

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
