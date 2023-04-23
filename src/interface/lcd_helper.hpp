#ifndef LCD_HELPER_HPP

/* -------------------------------- Libraries ------------------------------- */
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

/* --------------------------------- Headers -------------------------------- */
#include "../../globals.h"
#include "../resources/icons.hpp"
#include "../utilities/battery_utilities.hpp"

namespace Display
{
    extern hd44780_I2Cexp gLCD;
    // Initialization for the display
    void initDisplay();

    // Sets display parameters to baseline.
    void readyDisplay();

    // Create and store custom chars on LCD driver memory.
    void initCustomChars();

    /* Due to Hitachi driver constraints on custom chars, battery indicator is
        re-created as necessary
    */
    void pickBatteryIcon();

} // namespace Display

#define LCD_HELPER_HPP
#endif