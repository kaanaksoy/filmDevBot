#ifndef MENU_HELPER_HPP
#define MENU_HELPER_HPP

#include <CMBMenu.hpp>
#include <LCD_I2C.h>

#include "../../globals.h"
#include "../resources/icons.hpp"
#include "../sensors/temp_sensor_helper.hpp"
#include "../utilities/battery_utilities.hpp"
#include "../interface/lcd_helper.hpp"

namespace MenuUI
{

    // Define menu options
    extern const char gMenuC41[];
    extern const char gMenuCustom[];

    // Define menu functionIDs
    enum MenuFID
    {
        MenuNone,
        MenuC41,
        MenuCustom
    };

    // create menu instance
    extern CMBMenu<2> gMenu;

    // Print the development tank temp at the same location every time;
    void printTempReadings(float tankTemp);

    // --- printMenuEntry | Menu Functions ---
    void printMenuEntry(const char *funcInfo);

    void createMenu();

    void refreshMenu();

} // namespace MenuUI
#endif