#pragma once

//Header Files
#include "globals.h"
#include "utilfuncs.h"

//Libraries
#define ENCODER_OPTIMIZE_INTERRUPTS
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
    #define LONG_PRESS_DUR 350

    //int gEncoderPrevPos = 0;

    enum EncoderInputType {
        EncoderNone,
        EncoderLeft,
        EncoderRight,
        EncoderEnter,
        EncoderExit
    };



    // Define Encoder Navigation types


    Encoder gEncoder(ENC_DT, ENC_CLK);

    // --- getCommand | Menu Navigation Function ---
    EncoderInputType getCommand(){
        EncoderInputType command = EncoderNone;
        unsigned long timeNow;
        //bool buttonPressed = false;
        //bool longPressActive = false;
        unsigned long buttonTimer = 0;

        int gEncoderCurrPos = gEncoder.readAndReset();
        // if (digitalRead(ENC_SW) == LOW)
        // {
        //     if (buttonPressed == false)
        //     {
        //     buttonPressed = true;
        //     buttonTimer = millis();
        //     }
        //     if ((millis() - buttonTimer > LONG_PRESS_DUR) && (!longPressActive))
        //     {
        //         timeNow = millis();
        //         longPressActive = true;
        //         Utils::millisDelay(timeNow, 250);
        //         return EncoderExit;
        //     }
        // } else
        // {
        //     if (buttonPressed == true)
        //     {
        //         if (longPressActive == true)
        //         {   
        //             longPressActive = false; 
        //         } else
        //         {
        //             timeNow = millis();
        //             Utils::millisDelay(timeNow, 100);
        //             return EncoderEnter;
        //         }
        //         buttonPressed = false;
        //     }
            
        // }
        if (digitalRead(ENC_SW) == LOW)
        {
            buttonTimer = millis();
            while (digitalRead(ENC_SW) == LOW)
            {
                if (millis() - buttonTimer > LONG_PRESS_DUR)
                {
                    command = EncoderExit;
                    return command;
                }              
                
            }
            command = EncoderEnter;
        }
        

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
