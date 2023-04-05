#pragma once

#include "Arduino.h"
#include "globals.h"
#include "menuHelper.h"

// UI Pins
#define BUZZER 5

namespace Utils{
    /*
    --- buzz | UI Helper Functions ---
    Simple buzzer control, used to simplify code.
    */
    void buzz(int repeatTimes){

        for (int i = 0; i < repeatTimes; i++)
        {
            tone(BUZZER, 4000, 125);
            if(repeatTimes < 2) return;
            delay(200);
        }
        
    }
    /*
    --- readyLCD | General Helper Functions ---
    */
    void readyLCD(){
        gLCD.clear();
        gLCD.setCursor(0,0);
    }
}