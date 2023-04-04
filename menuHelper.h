#pragma once

#include <CMBMenu.hpp>
#include "globals.h"
#include "tempSensorHelper.h"

#define LEFT_ARR_ICON_ADDR 1
#define ENTER_ICON_ADDR 2
#define RIGHT_ARR_ICON_ADDR 3
#define EXIT_ICON_ADDR 4
#define TANK_TEMP_ICON_ADDR 5

LCD_I2C gLCD(LCD_ADDR, 16, 2); 
namespace MenuUI
{
    // create LCD Instance
    
    // Define menu options
    const char gMenuColor[] PROGMEM =    {"  Color    "};
    const char gMenuC41[] PROGMEM =      {"   C-41    "};
    const char gMenuE6[] PROGMEM =       {"   E-6     "};
    const char gMenuBW[] PROGMEM =       {"   B&W     "};
    const char gMenuBWCustom[] PROGMEM = {"  Custom   "};

    // Define menu functionIDs
    enum MenuFID {
        MenuColor,
        MenuC41,
        MenuE6,
        MenuBW,
        MenuBWCustom
    };

    //create menu instance
    CMBMenu<5> gMenu;

    void initCustomChars(){
        gLCD.createChar(LEFT_ARR_ICON_ADDR, UI_Icons::leftArrowChar);
        gLCD.createChar(ENTER_ICON_ADDR, UI_Icons::enterChar);
        gLCD.createChar(RIGHT_ARR_ICON_ADDR, UI_Icons::rightArrowChar);
        gLCD.createChar(EXIT_ICON_ADDR, UI_Icons::exitChar);
        gLCD.createChar(TANK_TEMP_ICON_ADDR, UI_Icons::tankTempChar);
    }

    void initLCD(){
        gLCD.begin();
        gLCD.backlight();
    }

    void printTempReadings(float tankTemp){
        
        if (tankTemp == SENSOR_NOT_READY) return;
        gLCD.setCursor(12, 1);
        gLCD.print(tankTemp);
        
    }

    // --- printMenuEntry | Menu Functions ---
    void printMenuEntry(const char* funcInfo){
    String infoStr;
    MBHelper::stringFromPgm(funcInfo, infoStr);

    // Print menu option
    gLCD.clear();
    gLCD.setCursor(0,0);
    gLCD.print(infoStr);

    //Print navigation UI
    gLCD.setCursor(2, 1);
    gLCD.write(LEFT_ARR_ICON_ADDR);
    gLCD.setCursor(4,1);
    gLCD.write(ENTER_ICON_ADDR);
    gLCD.setCursor(6,1);
    gLCD.write(RIGHT_ARR_ICON_ADDR);
    gLCD.setCursor(8,1);
    gLCD.write(EXIT_ICON_ADDR);
    gLCD.setCursor(11, 1);
    gLCD.write(TANK_TEMP_ICON_ADDR);

    printTempReadings(TempSensors::getTankTemp());
    }

    void createMenu(){
        // Add Nodes to the menu
        gMenu.addNode(0, gMenuColor, MenuColor);
        gMenu.addNode(1, gMenuC41, MenuC41);
        gMenu.addNode(1, gMenuE6, MenuE6);

        gMenu.addNode(0, gMenuBW, MenuBW);
        gMenu.addNode(1, gMenuBWCustom, MenuBWCustom);

        initCustomChars();
        // Build & Print Menu
        const char* info;
        gMenu.buildMenu(info);
        gMenu.printMenu();
        // Print Current Menu Entry
        printMenuEntry(info);
    }


} // namespace MenuUI
