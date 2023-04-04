#pragma once

//Header Files
#include "globals.h"
#include "utilfuncs.h"

#define ENCODER_OPTIMIZE_INTERRUPTS

//Libraries
#include <Encoder.h> 

namespace enc
{
    Encoder gEncoder(ENC_DT, ENC_CLK); //Initialize Encoder

    // Define Encoder Navigation types
    enum EncoderInputType {
        EncoderNone,
        EncoderLeft,
        EncoderRight,
        EncoderEnter,
        EncoderExit
    };

    // --- getCommand | Menu Navigation Function ---
    EncoderInputType getCommand(EncoderInputType prevCommand){
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
                    Utils::buzz(1);
                    return command;
                }              
                
            }
            command = EncoderEnter;
            return command;
        }
        int gEncoderCurrPos = gEncoder.readAndReset();
        if (0 > gEncoderCurrPos)
        {
            command = EncoderLeft;
        } else if (0 < gEncoderCurrPos)
        {
            command = EncoderRight;
        }        
        return command;
    }
} // namespace enc
