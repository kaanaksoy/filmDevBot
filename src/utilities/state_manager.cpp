
#include "state_manager.hpp"
namespace StateManager
{
    void setOperationState(OperationStateType newState)
    {
        if (State.currentState != IDLE && newState == IDLE)
        {
            MenuUI::refreshMenu();
        }
        State.currentState = newState;
        redrawMenu = true;
        return;
    }
} // namespace StateManager
