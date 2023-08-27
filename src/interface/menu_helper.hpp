#ifndef MENU_HELPER_HPP
// Define the header guard symbol to prevent multiple inclusion
#define MENU_HELPER_HPP

/* -------------------------------- Libraries ------------------------------- */
#include <CMBMenu.hpp>
#include <LCD_I2C.h>

/* --------------------------------- Headers -------------------------------- */
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
    extern const char gMenuCustom[];

    // Function IDs that match with the menu options. Used to call the appropriate func onClick.
    enum MenuFID
    {
        MenuNone,
        MenuC41,
        MenuCustom,
        MenuMonitor,
    };

    // create menu instance (MenuNone is not provided to the user)
    extern CMBMenu<3> gMenu;

    // Print development tank tempoerature.
    void printTempReadings(float tankTemp);

    // Print battery charge indicator
    void printBatteryInfo();

    // Print the appropriate meny entry and accompanying navigation icons.
    void printMenuEntry(const char *funcInfo);

    // Creates an instance of the Menu type.
    void createMenu();

    // Refreshes the Current Menu display
    void refreshMenu();

} // namespace MenuUI
#endif