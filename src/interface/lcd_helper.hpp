#ifndef LCD_HELPER_HPP

/* -------------------------------- Libraries ------------------------------- */
#include <LCD_I2C.h>
/* --------------------------------- Headers -------------------------------- */
#include "../../globals.h"
#include "../resources/icons.hpp"
#include "../utilities/battery_utilities.hpp"

namespace Display
{
    extern LCD_I2C gLCD;

    // Initialization for the display
    void initDisplay();

    // Sets display parameters to baseline.
    void readyDisplay();

    // Create and store custom chars on LCD driver memory.
    void initCustomChars();

    /* Due to Hitachi driver constraints on custom chars, battery indicator is
        re-creatred as necessary
    */
    void pickBatteryIcon();

} // namespace Display

#define LCD_HELPER_HPP
#endif