#include "encoder_helper.hpp"

namespace SystemEncoder
{
    // Encoder gEncoder(ENC_DT, ENC_CLK); // Initialize Encoder

    // // --- getCommand | Menu Navigation Function ---
    // EncoderInputType getCommand(EncoderInputType prevCommand)
    // {
    //     EncoderInputType command = EncoderNone;
    //     unsigned long timeNow;

    //     if (digitalRead(ENC_SW) == LOW && prevCommand != EncoderExit)
    //     {
    //         timeNow = State.currentMillis;
    //         while (digitalRead(ENC_SW) == LOW)
    //         {
    //             if (State.currentMillis - timeNow > LONG_PRESS_DUR)
    //             {
    //                 command = EncoderExit;
    //                 Utils::buzz(2);
    //                 delay(400);
    //                 return command;
    //             }
    //         }
    //         command = EncoderEnter;
    //         Utils::buzz(1);
    //         return command;
    //     }
    //     else
    //     {
    //         int gEncoderCurrPos = gEncoder.readAndReset();
    //         if (0 > gEncoderCurrPos)
    //         {
    //             command = EncoderLeft;
    //         }
    //         else if (0 < gEncoderCurrPos)
    //         {
    //             command = EncoderRight;
    //         }
    //     }

    //     return command;
    // }

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
        }
    }

    void initEncoder()
    {
        // The encoder module has pullups built in.
        pinMode(ENC_CLK, INPUT);
        pinMode(ENC_DT, INPUT);
        pinMode(ENC_SW, INPUT_PULLUP);

        attachInterrupt(digitalPinToInterrupt(ENC_CLK), enc_ISR, CHANGE);
        attachInterrupt(digitalPinToInterrupt(ENC_DT), enc_ISR, CHANGE);
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
                unsigned long timeStamp = State.currentMillis;
                while (digitalRead(ENC_SW) == LOW)
                {
                    if (millis() > timeStamp + LONG_PRESS_DUR)
                    {
                        Utils::buzz(2);
                        // delay(100);
                        needDebounce = true;
                        return EncoderExit;
                    }
                }
                Utils::buzz(1);
                return EncoderEnter;
            }
        }

        // Left - Right movement of the encoder
        State.checkEncoder = false; // Reset flag

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

    void enc_ISR()
    {
        State.checkEncoder = true;
    }
} // namespace Encoder
