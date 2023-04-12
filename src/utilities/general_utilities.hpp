#ifndef GENERAL_UTILITIES_HPP
#define GENERAL_UTILITIES_HPP

#include "Arduino.h"
#include "../../globals.h"
#include "../interface/menu_helper.hpp"

namespace Utils
{

    /*
    --- buzz | UI Helper Functions ---
    Simple buzzer control, used to simplify code.
    */
    void buzz(int repeatTimes);

}

#endif