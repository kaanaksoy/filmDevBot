#ifndef LCD_HELPER_HPP
// Define the header guard symbol to prevent multiple inclusion
#define LCD_HELPER_HPP

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
    // Declare an instance of the hd44780_I2Cexp class for the LCD display
    extern hd44780_I2Cexp gLCD;

    // Initialization function for the display
    void initDisplay();

    // Function to set display parameters to baseline
    void readyDisplay();

    // Function to create and store custom characters on the LCD driver memory
    void initCustomChars();

    /* Due to Hitachi driver constraints on custom characters, the battery indicator
       is re-created as necessary
    */
    void pickBatteryIcon();

    // Basic function to print strings from an array in progmem
    inline void glcdPrint_P(const char *const *stringTableMember) __attribute__((always_inline));

} // namespace Display

#endif