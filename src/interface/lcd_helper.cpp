#include "lcd_helper.hpp"

namespace Display
{
    hd44780_I2Cexp gLCD; // Instantiate an object of the hd44780_I2Cexp class

    // Initialization function for the display
    void initDisplay()
    {
        gLCD.begin(LCD_COLS, LCD_ROWS); // Initialize the LCD display with the specified number of columns and rows
        gLCD.clear();                   // Clear the display's content
    }

    // Ready diplay for wriitng.
    void readyDisplay()
    {
        gLCD.clear();
        gLCD.setCursor(0, 0);
    }

    // Create and store custom chars on LCD driver memory.
    void initCustomChars()
    {
        // Create custom characters using data from the Icons namespace
        gLCD.createChar(ENTER_ICON_ADDR, Icons::enterCustomChar);
        gLCD.createChar(EXIT_ICON_ADDR, Icons::exitCustomChar);
        gLCD.createChar(TANK_TEMP_ICON_ADDR, Icons::tankTempCustomChar);
        gLCD.createChar(DWN_ARROW_ADDR, Icons::downArrowCustomChar);
        gLCD.createChar(UP_DOWN_ARROWS_ICON_ADDR, Icons::upDownArrows);
        pickBatteryIcon(); // Create battery icon based on battery level
    }

    /* Due to Hitachi driver constraints on custom chars, battery indicator is
        re-creatred as necessary
    */
    void pickBatteryIcon()
    {
        switch (State.batteryLevel)
        {
        case ChargeLevelType::BatteryDisconnected:
            gLCD.createChar(BATT_CHAR_ADDR, Icons::batteryDisconnectedCustomChar);
            break;
        case ChargeLevelType::FullCharge:
            gLCD.createChar(BATT_CHAR_ADDR, Icons::fullBatteryCustomChar);
            break;
        case ChargeLevelType::MidCharge:
            gLCD.createChar(BATT_CHAR_ADDR, Icons::midBatteryCustomChar);
            break;
        case ChargeLevelType::LowCharge:
            gLCD.createChar(BATT_CHAR_ADDR, Icons::lowBatteryCustomChar);
            break;
        case ChargeLevelType::VeryLowCharge:
            gLCD.createChar(BATT_CHAR_ADDR, Icons::veryLowBatteryCustomChar);
            break;
        default:
            break;
        }
    }

    // Compact function to allow printing from a string array in progmem to the lcd
    void glcdPrint_P(const char *const *stringTableMember)
    {
        strcpy_P(tmpStr, (char *)pgm_read_word((stringTableMember))); // Necessary casts and dereferencing, just copy.
        gLCD.print(tmpStr);
    }

} // namespace Display
