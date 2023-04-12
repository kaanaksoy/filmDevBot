#include "menu_helper.hpp"

namespace MenuUI
{
    LCD_I2C gLCD(LCD_ADDR, 16, 2);
    CMBMenu<5> gMenu;

    // Define menu options
    const char gMenuColor[] PROGMEM = {"  Color   "};
    const char gMenuC41[] PROGMEM = {"   C-41   "};
    const char gMenuE6[] PROGMEM = {"   E-6    "};
    const char gMenuBW[] PROGMEM = {"   B&W    "};
    const char gMenuBWCustom[] PROGMEM = {"  Custom  "};

    void initLCD()
    {
        gLCD.begin();
        gLCD.backlight();
    }

    // Due to constraints on custom chars, battery indicator is created at runtime
    void pickBattIcon()
    {
        static BatteryMonitor::batteryLevelType prevChargeLevel = BatteryMonitor::batteryLevelType::FullCharge;
        BatteryMonitor::batteryLevelType newChargeLevel = BatteryMonitor::measureChargeLevel();

        if (prevChargeLevel == newChargeLevel)
        {
            return;
        }

        switch (BatteryMonitor::measureChargeLevel())
        {
        case BatteryMonitor::batteryLevelType::FullCharge:
            gLCD.createChar_P(BATT_CHAR_ADDR, Icons::fullBatteryChar);
            break;
        case BatteryMonitor::batteryLevelType::MidCharge:
            gLCD.createChar_P(BATT_CHAR_ADDR, Icons::midBatteryChar);
            break;
        case BatteryMonitor::batteryLevelType::LowCharge:
            gLCD.createChar_P(BATT_CHAR_ADDR, Icons::lowBatteryChar);
            break;
        case BatteryMonitor::batteryLevelType::VeryLowCharge:
            gLCD.createChar_P(BATT_CHAR_ADDR, Icons::veryLowBatteryChar);
            break;
        default:
            break;
        }
    }

    void initCustomChars()
    {
        gLCD.createChar_P(LEFT_ARR_ICON_ADDR, Icons::leftArrowChar);
        gLCD.createChar_P(ENTER_ICON_ADDR, Icons::enterChar);
        gLCD.createChar_P(RIGHT_ARR_ICON_ADDR, Icons::rightArrowChar);
        gLCD.createChar_P(EXIT_ICON_ADDR, Icons::exitChar);
        gLCD.createChar_P(TANK_TEMP_ICON_ADDR, Icons::tankTempChar);
        pickBattIcon();
    }

    void printTempReadings(float tankTemp)
    {

        if (tankTemp == SENSOR_NOT_READY)
            return;
        gLCD.setCursor(12, 0);
        gLCD.print(tankTemp);
    }

    // --- printMenuEntry | Menu Functions ---
    void printMenuEntry(const char *funcInfo)
    {

        String infoStr;
        MBHelper::stringFromPgm(funcInfo, infoStr);

        // Print menu option
        gLCD.clear();
        gLCD.setCursor(1, 0);
        gLCD.print(infoStr);

        // Print navigation UI
        if (infoStr == "  Color   ")
        {
            gLCD.setCursor(7, 1);
            gLCD.write(RIGHT_ARR_ICON_ADDR);
        }
        else if (infoStr == "   C-41   ")
        {
            gLCD.setCursor(7, 1);
            gLCD.write(RIGHT_ARR_ICON_ADDR);
        }
        else if (infoStr == "   E-6    ")
        {
            gLCD.setCursor(3, 1);
            gLCD.write(LEFT_ARR_ICON_ADDR);
        }
        else if (infoStr == "   B&W    ")
        {
            gLCD.setCursor(3, 1);
            gLCD.write(LEFT_ARR_ICON_ADDR);
        }
        else if (infoStr == "  Custom  ")
        {
        }
        else
        {
            gLCD.setCursor(3, 1);
            gLCD.write(LEFT_ARR_ICON_ADDR);
            gLCD.setCursor(7, 1);
            gLCD.write(RIGHT_ARR_ICON_ADDR);
        }

        gLCD.setCursor(0, 0);
        gLCD.write(BATT_CHAR_ADDR);
        gLCD.setCursor(5, 1);
        gLCD.write(ENTER_ICON_ADDR);
        gLCD.setCursor(10, 1);
        gLCD.write(EXIT_ICON_ADDR);
        gLCD.setCursor(11, 0);
        gLCD.write(TANK_TEMP_ICON_ADDR);

        if ((StateManager::State.currentMillis - BatteryMonitor::lastBatteryCheckTime > BATT_CHECK_PERIOD * 60 * 1000) || BatteryMonitor::lastBatteryCheckTime == 0)
        {
            BatteryMonitor::lastBatteryCheckTime = StateManager::State.currentMillis; // Reset timer to check battery.
            pickBattIcon();
        }
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

        initCustomChars();
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
