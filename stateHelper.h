#pragma once

#include "globals.h"

namespace StateManager
{

    enum OperationStateType {
        IDLE,
        DEVELOPING,
        MONITORING
    };

    struct state {
        OperationStateType currentState;
        bool ledInUse;
        bool buzzerInUse;
    };

    state State = {IDLE, false, false};


    void setState(OperationStateType newState){
        DEBUG_PRINT("State Was: ");
        #ifdef DEBUG
        switch (State.currentState)
            {
            case IDLE:
            DEBUG_PRINT("IDLE");
            break;
            case DEVELOPING:
            DEBUG_PRINT("DEVELOPING");
            break;
            case MONITORING:
            DEBUG_PRINT("MONITORING");
            break;
            default:
            DEBUG_PRINT("NULL");
            DEBUG_TRACE();
            break;
            }
        #endif
        if (State.currentState != IDLE && newState == IDLE)
        { 
            MenuUI::refreshMenu();
        }
        State.currentState = newState;
        DEBUG_PRINT("State became: ");
        #ifdef DEBUG
            switch (State.currentState)
            {
            case IDLE:
            DEBUG_PRINT("IDLE");
            break;
            case DEVELOPING:
            DEBUG_PRINT("DEVELOPING");
            break;
            case MONITORING:
            DEBUG_PRINT("MONITORING");
            break;
            default:
            DEBUG_PRINT("NULL");
            DEBUG_TRACE();
            break;
            }
        #endif

        return;
    }
} // namespace StateManager
