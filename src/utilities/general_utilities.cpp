#include "general_utilities.hpp"

namespace Utils
{

    /*
    --- buzz | UI Helper Functions ---
    Simple buzzer control, used to simplify code.
    */
    void buzz(int repeatTimes)
    {

        for (int i = 0; i < repeatTimes; i++)
        {
            tone(BUZZER, 4000, 125);
            if (repeatTimes < 2)
                return;
            delay(200);
        }
    }
    /*
    --- readyLCD | General Helper Functions ---
    */
    void readyLCD()
    {
        MenuUI::gLCD.clear();
        MenuUI::gLCD.setCursor(0, 0);
    }

}