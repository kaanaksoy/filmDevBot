#include "lcd_helper.hpp"

namespace Display
{
    LCD_I2C gLCD(LCD_ADDR, LCD_COLS, LCD_ROWS);

    // Initialization for the display
    void initDisplay()
    {
        gLCD.begin();
        gLCD.backlight();
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
        // gLCD.createChar_P(LEFT_ARR_ICON_ADDR, Icons::leftArrowChar);
        gLCD.createChar_P(ENTER_ICON_ADDR, Icons::enterCustomChar);
        // gLCD.createChar_P(RIGHT_ARR_ICON_ADDR, Icons::rightArrowChar);
        gLCD.createChar_P(EXIT_ICON_ADDR, Icons::exitCustomChar);
        gLCD.createChar_P(TANK_TEMP_ICON_ADDR, Icons::tankTempCustomChar);
        pickBatteryIcon();
    }

    /* Due to Hitachi driver constraints on custom chars, battery indicator is
        re-creatred as necessary
    */
    void pickBatteryIcon()
    {
        static BatteryMonitor::batteryLevelType prevChargeLevel = BatteryMonitor::batteryLevelType::FullCharge;
        BatteryMonitor::batteryLevelType newChargeLevel = BatteryMonitor::measureChargeLevel();

        if (prevChargeLevel == newChargeLevel)
        {
            return;
        }

        switch (BatteryMonitor::measureChargeLevel())
        {
        case BatteryMonitor::batteryLevelType::BatteryDisconnected:
            gLCD.createChar_P(BATT_CHAR_ADDR, Icons::batteryDisconnectedCustomChar);
            break;
        case BatteryMonitor::batteryLevelType::FullCharge:
            gLCD.createChar_P(BATT_CHAR_ADDR, Icons::fullBatteryCustomChar);
            break;
        case BatteryMonitor::batteryLevelType::MidCharge:
            gLCD.createChar_P(BATT_CHAR_ADDR, Icons::midBatteryCustomChar);
            break;
        case BatteryMonitor::batteryLevelType::LowCharge:
            gLCD.createChar_P(BATT_CHAR_ADDR, Icons::lowBatteryCustomChar);
            break;
        case BatteryMonitor::batteryLevelType::VeryLowCharge:
            gLCD.createChar_P(BATT_CHAR_ADDR, Icons::veryLowBatteryCustomChar);
            break;
        default:
            break;
        }
    }

} // namespace Display
