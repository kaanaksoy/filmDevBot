#pragma once

#include <CMBMenu.hpp>
#include "globals.h"
#include "tempSensorHelper.h"

namespace MenuUI
{
    
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

    void printTempReadings(float tankTemp, float bathTemp = -1){

        //Print Tank Temp
        gLCD.setCursor(12, 1);
        gLCD.print(tankTemp);

        if (bathTemp != -1)
        {
            //Print Bath Temp
        }
        
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
    gLCD.createChar(1, UI_Icons::leftArrowChar);
    gLCD.setCursor(2, 1);
    gLCD.write(1);

    gLCD.createChar(2, UI_Icons::enterChar);
    gLCD.setCursor(4,1);
    gLCD.write(2);

    gLCD.createChar(3, UI_Icons::rightArrowChar);
    gLCD.setCursor(6,1);
    gLCD.write(3);

    gLCD.createChar(4, UI_Icons::exitChar);
    gLCD.setCursor(8,1);
    gLCD.write(4);

    gLCD.createChar(5, UI_Icons::tankTempChar);
    gLCD.setCursor(11, 1);
    gLCD.write(5);

    printTempReadings(TempSensors::getTankTemp());
    }

    void createMenu(){
        // Add Nodes to the menu
        gMenu.addNode(0, gMenuColor, MenuColor);
        gMenu.addNode(1, gMenuC41, MenuC41);
        gMenu.addNode(1, gMenuE6, MenuE6);

        gMenu.addNode(0, gMenuBW, MenuBW);
        gMenu.addNode(1, gMenuBWCustom, MenuBWCustom);

        // Build & Print Menu
        const char* info;
        gMenu.buildMenu(info);
        gMenu.printMenu();
        // Print Current Menu Entry
        printMenuEntry(info);
    }


} // namespace MenuUI
