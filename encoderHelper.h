#pragma once

//Libraries
#include <Encoder.h> 
//Header Files
#include "globals.h"
#include "utilfuncs.h"

//Encoder Pins 
/* 
(Using Encoder Breakout Board with pullip resistors. 
If you dont have pullup resistors, you can enable the built in ones.
*/
#define ENC_CLK 2 // Encoder In B
#define ENC_DT 3 // Encoder In A
#define ENC_SW 4 // Encoder Button
#define LONG_PRESS_DUR 500 // Button long press duration

namespace SystemEncoder
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
                    Utils::buzz(2);
                    delay(400);
                    return command;
                }              
                
            }
            command = EncoderEnter;
            Utils::buzz(1);
            return command;
        }
        else{
            int gEncoderCurrPos = gEncoder.readAndReset();
            if (0 > gEncoderCurrPos)
            {
                command = EncoderLeft;
            } else if (0 < gEncoderCurrPos)
            {
                command = EncoderRight;
            }
        }
                
        return command;
    }

    EncoderInputType encoderAwaitConfirm(){
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
