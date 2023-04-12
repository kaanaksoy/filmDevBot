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

    struct state
    {
        OperationStateType currentState;
        bool ledInUse;
        bool buzzerInUse;
        unsigned long currMillis;
    };

    extern state State;

    void setState(OperationStateType newState);
} // namespace StateManager

#endif