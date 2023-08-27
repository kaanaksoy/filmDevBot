
#include "state_manager.hpp"
namespace StateManager
{
    // Set the current operation state
    void setOperationState(OperationStateType newState)
    {
        // Check if transitioning from a non-IDLE state to IDLE state
        if (State.currentState != IDLE && newState == IDLE)
        {
            // If transitioning to IDLE, refresh the menu
            MenuUI::refreshMenu();
        }
        // Update the current state to the new state
        State.currentState = newState;

        // Set flag to redraw the menu
        redrawMenu = true;

        return;
    }
} // namespace StateManager
