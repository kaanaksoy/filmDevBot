#ifndef ICONS_HPP
#define ICONS_HPP

// Locations to store the custom icons on the HITACHI driver for 16X02 LCD displays.
//  for more information please check createChar() documentation
#define LEFT_ARR_ICON_ADDR (char)0b111100  // character built into LCD driver.
#define RIGHT_ARR_ICON_ADDR (char)0b111110 // char built into LCD driver.

#define TEMP_SYMBOL_ICON_ADDR (char)0b11011111
#define CELCIUS_SYMBOL_ICON_ADDR (char)0b1000011

#define ENTER_ICON_ADDR 1
#define EXIT_ICON_ADDR 2
#define TANK_TEMP_ICON_ADDR 3
#define BATT_CHAR_ADDR 6

#define UP_ARROW_ADDR (char)0b01011110
#define HOR_LINE_ADDR (char)0b00101101
#define DWN_ARROW_ADDR 7
#define DOT_ICON_ADDR (char)0b10100101
#define UP_DOWN_ARROWS_ICON_ADDR 8

namespace Icons
{

    const byte enterCustomChar[8] PROGMEM = {
        B00001,
        B00001,
        B00001,
        B00101,
        B01101,
        B11111,
        B01100,
        B00100};

    const byte exitCustomChar[8] PROGMEM = {
        B00000,
        B00000,
        B10001,
        B01010,
        B00100,
        B01010,
        B10001,
        B00000};

    const byte bathTempCustomChar[8] PROGMEM = {
        B00010,
        B00101,
        B00010,
        B11000,
        B10100,
        B11000,
        B10100,
        B11000};

    const byte tankTempCustomChar[8] PROGMEM = {
        B00010,
        B00101,
        B00010,
        B00000,
        B11100,
        B01000,
        B01000,
        B01000};

    const byte veryLowBatteryCustomChar[8] PROGMEM = {
        B01110,
        B10001,
        B10001,
        B10001,
        B10001,
        B10001,
        B10001,
        B11111};

    const byte lowBatteryCustomChar[8] PROGMEM = {
        B01110,
        B10001,
        B10001,
        B10001,
        B10001,
        B11111,
        B11111,
        B11111};

    const byte midBatteryCustomChar[8] PROGMEM = {
        B01110,
        B10001,
        B10001,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111};

    const byte fullBatteryCustomChar[8] PROGMEM = {
        B01110,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111};

    const byte batteryDisconnectedCustomChar[8] PROGMEM = {
        B01110,
        B11111,
        B01110,
        B10101,
        B11011,
        B10101,
        B01110,
        B11111};

    const byte downArrowCustomChar[8] PROGMEM = {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B10001,
        B01010,
        B00100};

    const byte upDownArrows[8] PROGMEM = {
        B00000,
        B00100,
        B01110,
        B11111,
        B00000,
        B11111,
        B01110,
        B00100};
} // namespace Icons
#endif