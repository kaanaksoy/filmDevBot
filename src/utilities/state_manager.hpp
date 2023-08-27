#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

/* --------------------------------- Headers -------------------------------- */
#include "../../globals.h"
#include "../../debugUtils.h"
#include "../interface/menu_helper.hpp"
#include "battery_utilities.hpp"

namespace StateManager
{
    // Function to set the operation state.
    void setOperationState(OperationStateType newState);
} // namespace StateManager

#endif