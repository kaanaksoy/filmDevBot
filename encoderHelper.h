#pragma once

//Header Files
#include "globals.h"
#include "utilfuncs.h"

#define ENCODER_OPTIMIZE_INTERRUPTS

//Libraries
#include <Encoder.h> 

namespace enc
{
    //Encoder Pins 
    /* 
    (Using Encoder Breakout Board with pullip resistors. 
    If you dont have pullup resistors, you can enable the built in ones.
    */
    #define ENC_CLK 2 // Encoder In B
    #define ENC_DT 3 // Encoder In A
    #define ENC_SW 4 // Encoder Button
    #define LONG_PRESS_DUR 250

    // Define Encoder Navigation types
    enum EncoderInputType {
        EncoderNone,
        EncoderLeft,
        EncoderRight,
        EncoderEnter,
        EncoderExit
    };

    Encoder gEncoder(ENC_DT, ENC_CLK); //Initialize Encoder

    // --- getCommand | Menu Navigation Function ---
    EncoderInputType getCommand(){
        EncoderInputType command = EncoderNone;
        unsigned long timeNow;

        if (digitalRead(ENC_SW) == LOW)
        {
            timeNow = millis();
            while (digitalRead(ENC_SW) == LOW)
            {
                if (millis() - timeNow > LONG_PRESS_DUR)
                {
                    command = EncoderExit;
                    return command;
                }              
                
            }
            command = EncoderEnter;
            return command;
        }
        
        int gEncoderCurrPos = gEncoder.readAndReset();

        if (0 > gEncoderCurrPos+1)
        {
            command = EncoderLeft;
        } else if (0 < gEncoderCurrPos-1)
        {
            command = EncoderRight;
        }        
        return command;
    }
} // namespace enc
