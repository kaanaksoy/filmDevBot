#pragma once

#include "Arduino.h"
#include "globals.h"
#include "debugUtils.h"
#include "icons.h"
#include "filmDevUtils.h"
#include "encoderHelper.h"
#include "ledHelper.h"
#include "stateHelper.h"


namespace DevelopFilm
{
    // --- ColorC41 | Film Development Functions ---
void ColorC41(){
    DEBUG_PRINT("ColorC41(): Started now.")
    StateManager::setState(StateManager::DEVELOPING);

    int8_t pushPullValue = 0;
    uint16_t duration;

    Utils::readyLCD();
    gLCD.print("C-41 Development");
    SystemEncoder::EncoderInputType command = SystemEncoder::EncoderNone;
    while (command != SystemEncoder::EncoderEnter)
    {   
        gLCD.setCursor(0,1);
        //manually adding sign & usign abs allows the text to not expand and shrink when it becomes positive or negative.
        gLCD.print(
            String("Psh/Pll: ") + 
            (0 > pushPullValue ? "-" : "+") + 
            String(abs(pushPullValue))
            ); 
        command = SystemEncoder::getCommand(command); // Determine command.
        switch (command)
        {
            case SystemEncoder::EncoderLeft:
                pushPullValue--;
                if (pushPullValue < MAX_PULL) pushPullValue = MAX_PULL;
                break;
            case SystemEncoder::EncoderRight:
                pushPullValue++;
                if (pushPullValue > MAX_PUSH) pushPullValue = MAX_PUSH;
                break;
            case SystemEncoder::EncoderExit:
                StateManager::setState(StateManager::IDLE);
                return;
            case SystemEncoder::EncoderNone:
            default:
                break;
        }
        delay(400);
    }

    Utils::readyLCD();

    switch (pushPullValue)
    {
    case -1:
        duration = PULL_ONE_DUR;
        gLCD.print("C-41, -1 Pull");
        break;
    case 0:
        duration = STD_DEV_DUR;
        gLCD.print("C-41, +0 Push");
        break;
    case 1:
        duration = PUSH_ONE_DUR;
        gLCD.print("C-41, +1 Push");
        break;
    case 2:
        duration = PUSH_TWO_DUR;
        gLCD.print("C-41, +2 Push");
        break;
    case 3:
        duration = PUSH_THR_DUR;
        gLCD.print("C-41, +3 Push");
        break;
    default:
        break;
    }

    gLCD.setCursor(0,1);
    gLCD.setCursor(3,1);
    gLCD.write(ENTER_ICON_ADDR);
    gLCD.setCursor(5, 1);
    gLCD.print("to start...");
    
    switch (SystemEncoder::encoderAwaitConfirm())
    {
    case SystemEncoder::EncoderEnter:
        /* code */
        break;
    case SystemEncoder::EncoderExit:
        StateManager::setState(StateManager::IDLE);
        return;
    default:
        break;
    }

    Utils::readyLCD();
    gLCD.print("Running.");
    gLCD.setCursor(11, 0);
    gLCD.write(TANK_TEMP_ICON_ADDR);
    
    FilmDevHelpers::develop(duration, 10, 6, 30);
    DEBUG_PRINT("ColorC41(): Just finished developing");
    
    Utils::readyLCD();
    
    gLCD.print("Dev Finished");
    gLCD.setCursor(0,1);
    gLCD.createChar(ENTER_ICON_ADDR, Icons::enterChar);
    gLCD.setCursor(0,1);
    gLCD.write(ENTER_ICON_ADDR);
    gLCD.setCursor(2, 1);
    gLCD.print("to strt fixing");

    DEBUG_PRINT("ColorC41(): Starting Fixing.")
    
    switch (SystemEncoder::encoderAwaitConfirm())
    {
    case SystemEncoder::EncoderEnter:
        /* code */
        break;
    case SystemEncoder::EncoderExit:
        StateManager::setState(StateManager::IDLE);
        return;
    default:
        break;
    }

    Utils::readyLCD();
    gLCD.print("Fixing...");
    gLCD.setCursor(11, 0);
    gLCD.write(TANK_TEMP_ICON_ADDR);
    FilmDevHelpers::fix(FIXING_TIME);

    Utils::readyLCD();
    gLCD.print("Process Finished");
    gLCD.setCursor(0,1);
    gLCD.createChar(ENTER_ICON_ADDR, Icons::enterChar);
    gLCD.setCursor(3,1);
    gLCD.write(ENTER_ICON_ADDR);
    gLCD.setCursor(5, 1);
    gLCD.print("to exit...");

    switch (SystemEncoder::encoderAwaitConfirm())
    {
    case SystemEncoder::EncoderEnter:
        /* code */
        break;
    case SystemEncoder::EncoderExit:
        StateManager::setState(StateManager::IDLE);
        return;
    default:
        break;
    }

    StateManager::setState(StateManager::IDLE);
    return;
}


void ColorE6(){
    StateManager::setState(StateManager::DEVELOPING);
    //StatusLED::blink(5);
    StateManager::setState(StateManager::IDLE);
    return;
}

void BWCustom(){
    StateManager::setState(StateManager::DEVELOPING);
    StateManager::setState(StateManager::IDLE);
    return;
}
} // namespace DevelopFilm
