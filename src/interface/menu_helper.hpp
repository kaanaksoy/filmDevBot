#ifndef MENU_HELPER_HPP
#define MENU_HELPER_HPP

#include <CMBMenu.hpp>
#include <LCD_I2C.h>

#include "../../globals.h"
#include "../../icons.h"
#include "../sensors/temp_sensor_helper.hpp"
#include "../utilities/general_utilities.hpp"
#include "../utilities/battery_utilities.hpp"

namespace MenuUI
{
    // create LCD Instance
    extern LCD_I2C gLCD;

    // Define menu options
    extern const char gMenuColor[];
    extern const char gMenuC41[];
    extern const char gMenuE6[];
    extern const char gMenuBW[];
    extern const char gMenuBWCustom[];

    // Define menu functionIDs
    enum MenuFID
    {
        MenuColor,
        MenuC41,
        MenuE6,
        MenuBW,
        MenuBWCustom
    };

    // create menu instance
    extern CMBMenu<5> gMenu;

    void initLCD();

    // Due to constraints on custom chars, battery indicator is created at runtime
    void pickBattIcon();

    void initCustomChars();

    void printTempReadings(float tankTemp);

    // --- printMenuEntry | Menu Functions ---
    void printMenuEntry(const char *funcInfo);

    void createMenu();

    void refreshMenu();

} // namespace MenuUI
#endif