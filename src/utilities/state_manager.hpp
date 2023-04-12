#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include "../../globals.h"
#include "../../debugUtils.h"
#include "../interface/menu_helper.hpp"

namespace StateManager
{

    enum OperationStateType
    {
        IDLE,
        DEVELOPING,
        MONITORING
    };

    struct StateType
    {
        // State variable
        OperationStateType currentState;
        // Make sure that no function can use the led if another function is doing so.
        bool ledInUse;
        // Follows LED
        bool buzzerInUse;
        // Track global time. updated every loop of the main loop.
        unsigned long currentMillis;
    };

    extern StateType State;

    void setState(OperationStateType newState);
} // namespace StateManager

#endif