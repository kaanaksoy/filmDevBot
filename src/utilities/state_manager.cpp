
#include "state_manager.hpp"
namespace StateManager
{
    void setOperationState(OperationStateType newState)
    {
        // #ifndef DEBUG
        // #define DEBUG
        //         Serial.println("State Was: ");
        //         switch (State.currentState)
        //         {
        //         case IDLE:
        //             Serial.println("IDLE");
        //             break;
        //         case DEVELOPING:
        //             Serial.println("DEVELOPING");
        //             break;
        //         case MONITORING:
        //             Serial.println("MONITORING");
        //             break;
        //         case INDEVELOPMENU:
        //             Serial.println("INDEVELOPMENU");
        //             break;
        //         default:
        //             Serial.println("NULL");
        //             break;
        //         }
        //         Serial.println("State became: ");
        //         switch (newState)
        //         {
        //         case IDLE:
        //             Serial.println("IDLE");
        //             break;
        //         case DEVELOPING:
        //             Serial.println("DEVELOPING");
        //             break;
        //         case MONITORING:
        //             Serial.println("MONITORING");
        //             break;
        //         case INDEVELOPMENU:
        //             Serial.println("INDEVELOPMENU");
        //             break;
        //         default:
        //             Serial.println("NULL");
        //             break;
        //         }
        // #endif
        if (State.currentState != IDLE && newState == IDLE)
        {
            MenuUI::refreshMenu();
        }
        State.currentState = newState;
        redrawMenu = true;
        return;
    }
} // namespace StateManager
