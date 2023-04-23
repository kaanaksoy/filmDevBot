#include "menu_helper.hpp"

namespace MenuUI
{
    CMBMenu<5> gMenu;

    // Define menu options
    const char gMenuColor[] PROGMEM = {"  Color   "};
    const char gMenuC41[] PROGMEM = {"   C-41   "};
    const char gMenuE6[] PROGMEM = {"   E-6    "};
    const char gMenuBW[] PROGMEM = {"   B&W    "};
    const char gMenuBWCustom[] PROGMEM = {"  Custom  "};

    // Print the development tank temp at the same location every time;
    void printTempReadings(float tankTemp)
    {

        if (tankTemp == SENSOR_NOT_READY)
            return;
        Display::gLCD.setCursor(12, 0);
        Display::gLCD.print(tankTemp);
    }

    // --- printMenuEntry | Menu Functions ---
    void printMenuEntry(const char *funcInfo)
    {

        String infoStr;
        MBHelper::stringFromPgm(funcInfo, infoStr);

        // Print menu option
        Display::gLCD.clear();
        Display::gLCD.setCursor(1, 0);
        Display::gLCD.print(infoStr);

        // Print navigation UI
        if (infoStr == "  Color   ")
        {
            Display::gLCD.setCursor(7, 1);
            Display::gLCD.write(RIGHT_ARR_ICON_ADDR);
        }
        else if (infoStr == "   C-41   ")
        {
            Display::gLCD.setCursor(7, 1);
            Display::gLCD.write(RIGHT_ARR_ICON_ADDR);
        }
        else if (infoStr == "   E-6    ")
        {
            Display::gLCD.setCursor(3, 1);
            Display::gLCD.write(LEFT_ARR_ICON_ADDR);
        }
        else if (infoStr == "   B&W    ")
        {
            Display::gLCD.setCursor(3, 1);
            Display::gLCD.write(LEFT_ARR_ICON_ADDR);
        }
        else if (infoStr == "  Custom  ")
        {
        }
        else
        {
            Display::gLCD.setCursor(3, 1);
            Display::gLCD.write(LEFT_ARR_ICON_ADDR);
            Display::gLCD.setCursor(7, 1);
            Display::gLCD.write(RIGHT_ARR_ICON_ADDR);
        }

        Display::pickBatteryIcon();
        Display::gLCD.setCursor(0, 0);
        Display::gLCD.write(BATT_CHAR_ADDR);
        Display::gLCD.setCursor(5, 1);
        Display::gLCD.write(ENTER_ICON_ADDR);
        Display::gLCD.setCursor(10, 1);
        Display::gLCD.write(EXIT_ICON_ADDR);
        Display::gLCD.setCursor(11, 0);
        Display::gLCD.write(TANK_TEMP_ICON_ADDR);
        printTempReadings(TempSensors::getTankTemp());
    }

    void createMenu()
    {
        // Add Nodes to the menu
        gMenu.addNode(0, gMenuColor, MenuColor);
        gMenu.addNode(1, gMenuC41, MenuC41);
        gMenu.addNode(1, gMenuE6, MenuE6);

        gMenu.addNode(0, gMenuBW, MenuBW);
        gMenu.addNode(1, gMenuBWCustom, MenuBWCustom);

        Display::initCustomChars();
        // Build & Print Menu
        const char *info;
        gMenu.buildMenu(info);
        gMenu.printMenu();
        // Print Current Menu Entry
        printMenuEntry(info);
    }

    void refreshMenu()
    {
        const char *info;
        gMenu.getInfo(info);
        // gMenu.buildMenu(info);
        // gMenu.printMenu();
        // Print Current Menu Entry
        printMenuEntry(info);
    }

} // namespace MenuUI
