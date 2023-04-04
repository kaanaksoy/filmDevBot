#pragma once

#include "Arduino.h"


namespace Utils{
    /*
    --- buzz | UI Helper Functions ---
    Simple buzzer control, used to simplify code.
    */
    void buzz(int repeatTimes){

        tone(BUZZER, 4000, 125);
    }
    /*
    --- millisDelay | General Helper Functions ---
    Simple function for non-blocking delay.
    */
    void millisDelay(unsigned long timeCalled, uint16_t duration){
        // while(millis() < timeCalled + duration){
        //     // wait "duration" ms        
        // }
        if (millis() < timeCalled + duration) return;
    }
}