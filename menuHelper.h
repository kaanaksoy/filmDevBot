#pragma once

#include <CMBMenu.hpp>
#include "globals.h"

namespace MenuUI
{
    // Define menu options
    const char gMenuColor[] PROGMEM =    {"      Color     "};
    const char gMenuC41[] PROGMEM =      {"       C-41     "};
    const char gMenuE6[] PROGMEM =       {"       E-6      "};
    const char gMenuBW[] PROGMEM =       {"       B&W      "};
    const char gMenuBWCustom[] PROGMEM = {"      Custom    "};

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


    // --- printMenuEntry | Menu Functions ---
    void printMenuEntry(const char* funcInfo){
    String infoStr;
    MBHelper::stringFromPgm(funcInfo, infoStr);

    gLCD.clear();
    gLCD.setCursor(0,0);
    gLCD.print(infoStr);

    gLCD.createChar(1, UI_Icons::leftArrowChar);
    gLCD.setCursor(6, 1);
    gLCD.write(1);

    gLCD.createChar(2, UI_Icons::enterChar);
    gLCD.setCursor(8,1);
    gLCD.write(2);

    gLCD.createChar(3, UI_Icons::rightArrowChar);
    gLCD.setCursor(10,1);
    gLCD.write(3);

    gLCD.createChar(4, UI_Icons::exitChar);
    gLCD.setCursor(14,1);
    gLCD.write(4);
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
