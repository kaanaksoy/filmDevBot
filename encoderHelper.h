#pragma once

//Header Files
#include "globals.h"
#include "utilfuncs.h"

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



    int gEncoderPrevPos = 0;
    bool buttonActive = false;
    bool longPressActive = false;

    long buttonTimer = 0;
    long longPressTime = 500;

    // Define Encoder Navigation types


    Encoder gEncoder(ENC_DT, ENC_CLK);

    // --- getCommand | Menu Navigation Function ---
    EncoderInputType getCommand(){
    EncoderInputType command = EncoderNone;
    unsigned long timeNow;


    if (gEncoderPrevPos > gEncoder.read())
    {
        command = EncoderLeft;
    } else if (gEncoderPrevPos < gEncoder.read())
    {
        command = EncoderRight;
    } 
    
    if (digitalRead(ENC_SW) == LOW)
    {
        if (buttonActive == false)
        {
        buttonActive = true;
        buttonTimer = millis();
        }
        if ((millis() - buttonTimer > longPressTime) && (!longPressActive))
        {
            timeNow = millis();
            longPressActive = true;
            command = EncoderExit;
            Utils::millisDelay(timeNow, 250);
        }
    } else
    {
        if (buttonActive == true)
        {
        if (longPressActive == true)
        {   
            longPressActive = false; 
        } else
        {
            timeNow = millis();
            command = EncoderEnter;
            Utils::millisDelay(timeNow, 100);
        }
        buttonActive = false;
        }
        
    }
    
    gEncoderPrevPos = gEncoder.read();
    return command;
    }
} // namespace enc
