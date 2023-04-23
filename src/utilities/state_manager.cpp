#include "state_manager.hpp"

namespace StateManager
{
    void setOperationState(OperationStateType newState)
    {
#ifdef DEBUG
        DEBUG_PRINT("State Was: ");
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
#ifdef DEBUG
        DEBUG_PRINT("State became: ");
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
