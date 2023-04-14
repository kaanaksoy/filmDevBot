#ifndef ICONS_HPP
#define ICONS_HPP

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

    // // TODO will be replaced with ASCII char 60
    // const byte leftArrowChar[8] PROGMEM = {
    //     B00000,
    //     B00010,
    //     B00110,
    //     B01111,
    //     B11111,
    //     B01111,
    //     B00110,
    //     B00010};

    // // TODO will be replaced with ASCII char 62
    // const byte rightArrowChar[8] PROGMEM = {
    //     B00000,
    //     B01000,
    //     B01100,
    //     B11110,
    //     B11111,
    //     B11110,
    //     B01100,
    //     B01000};

} // namespace Icons
#endif