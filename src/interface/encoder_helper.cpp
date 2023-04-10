#include "encoder_helper.hpp"

namespace SystemEncoder
{
    Encoder gEncoder(ENC_DT, ENC_CLK); // Initialize Encoder

    // --- getCommand | Menu Navigation Function ---
    EncoderInputType getCommand(EncoderInputType prevCommand)
    {
        EncoderInputType command = EncoderNone;
        unsigned long timeNow;

        if (digitalRead(ENC_SW) == LOW && prevCommand != EncoderExit)
        {
            timeNow = millis();
            while (digitalRead(ENC_SW) == LOW)
            {
                if (millis() - timeNow > LONG_PRESS_DUR)
                {
                    command = EncoderExit;
                    Utils::buzz(2);
                    delay(400);
                    return command;
                }
            }
            command = EncoderEnter;
            Utils::buzz(1);
            return command;
        }
        else
        {
            int gEncoderCurrPos = gEncoder.readAndReset();
            if (0 > gEncoderCurrPos)
            {
                command = EncoderLeft;
            }
            else if (0 < gEncoderCurrPos)
            {
                command = EncoderRight;
            }
        }

        return command;
    }

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
} // namespace Encoder
