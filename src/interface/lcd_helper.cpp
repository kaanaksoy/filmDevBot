#include "lcd_helper.hpp"

namespace Display
{
    hd44780_I2Cexp gLCD;

    // Initialization for the display
    void initDisplay()
    {
        gLCD.begin(LCD_COLS, LCD_ROWS);
        gLCD.clear();
    }

    /*
    --- readyLCD | General Helper Functions ---
    */
    void readyDisplay()
    {
        gLCD.clear();
        gLCD.setCursor(0, 0);
    }

    // Create and store custom chars on LCD driver memory.
    void initCustomChars()
    {
        gLCD.createChar(ENTER_ICON_ADDR, Icons::enterCustomChar);
        gLCD.createChar(EXIT_ICON_ADDR, Icons::exitCustomChar);
        gLCD.createChar(TANK_TEMP_ICON_ADDR, Icons::tankTempCustomChar);
        gLCD.createChar(DWN_ARROW_ADDR, Icons::downArrowCustomChar);
        gLCD.createChar(UP_DOWN_ARROWS_ICON_ADDR, Icons::upDownArrows);
        pickBatteryIcon();
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

    // Compact function to allow printing from a sting array in progmem to the lcd
    void glcdPrint_P(const char *const *stringTableMember)
    {
        strcpy_P(tmpStr, (char *)pgm_read_word((stringTableMember))); // Necessary casts and dereferencing, just copy.
        gLCD.print(tmpStr);
    }

} // namespace Display
