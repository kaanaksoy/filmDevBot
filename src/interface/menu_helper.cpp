#include "menu_helper.hpp"

namespace MenuUI
{
    CMBMenu<3> gMenu; // Create a menu instance with a capacity for 3 items.

    // Define menu options as PROGMEM strings
    const char gMenuC41[] PROGMEM = {"   C-41   "};
    const char gMenuCustom[] PROGMEM = {"  Custom  "};
    const char gMenuMonitor[] PROGMEM = {" Monitor"};

    // Print the development tank temperature at a specific location on the LCD display.
    void printTempReadings(float tankTemp)
    {
        // Check if the sensor reading is valid
        if (tankTemp == SENSOR_NOT_READY)
            return;

        // Set the cursor position and print the tank temperature
        Display::gLCD.setCursor(12, 0);
        Display::gLCD.print(tankTemp);
    }

    // Print battery information on the menu display.
    void printBatteryInfo()
    {
        Display::pickBatteryIcon();          // Set the appropriate battery icon
        Display::gLCD.setCursor(0, 0);       // Set cursor position
        Display::gLCD.write(BATT_CHAR_ADDR); // Display the battery icon
    }

    // Print a menu entry with navigation indicators and other UI elements.
    void printMenuEntry(const char *funcInfo)
    {

        String infoStr;
        MBHelper::stringFromPgm(funcInfo, infoStr); // Convert PROGMEM string to regular string

        // Clear the display and set cursor positions for printing menu option and navigation UI
        Display::gLCD.clear();
        Display::gLCD.setCursor(1, 0);
        Display::gLCD.print(infoStr);

        // Determine the appropriate navigation indicators based on the selected menu option
        if (infoStr == F("   C-41   "))
        {
            Display::gLCD.setCursor(7, 1);
            Display::gLCD.write(RIGHT_ARR_ICON_ADDR);
        }
        else if (infoStr == F(" Monitor"))
        {
            Display::gLCD.setCursor(3, 1);
            Display::gLCD.write(LEFT_ARR_ICON_ADDR);
        }
        else
        {
            Display::gLCD.setCursor(3, 1);
            Display::gLCD.write(LEFT_ARR_ICON_ADDR);
            Display::gLCD.setCursor(7, 1);
            Display::gLCD.write(RIGHT_ARR_ICON_ADDR);
        }

        // Print additional UI elements
        Display::gLCD.setCursor(5, 1);
        Display::gLCD.write(ENTER_ICON_ADDR);
        Display::gLCD.setCursor(10, 1);
        Display::gLCD.write(EXIT_ICON_ADDR);
        Display::gLCD.setCursor(11, 0);
        Display::gLCD.write(TANK_TEMP_ICON_ADDR);
        printTempReadings(TempSensors::getTankTemp()); // Print tank temperature
    }

    // Create the menu structure and items
    void createMenu()
    {
        // Add Nodes to the menu
        gMenu.addNode(0, gMenuC41, MenuC41);
        gMenu.addNode(0, gMenuCustom, MenuCustom);
        gMenu.addNode(0, gMenuMonitor, MenuMonitor);

        Display::initCustomChars(); // Initialize custom characters for the display
        const char *info;
        gMenu.buildMenu(info); // Build the menu.
        gMenu.printMenu();     // Print the menu.
        printMenuEntry(info);  // Print the menu entry
    }

    // Refresh the menu display with the current menu entry
    void refreshMenu()
    {
        const char *info;
        gMenu.getInfo(info);  // Get the current menu entry information
        printMenuEntry(info); // Print the updated menu entry
    }

} // namespace MenuUI
