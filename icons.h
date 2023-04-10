#pragma once

namespace Icons
{
  const byte leftArrowChar[8] PROGMEM = {
      B00000,
      B00010,
      B00110,
      B01111,
      B11111,
      B01111,
      B00110,
      B00010};

  const byte rightArrowChar[8] PROGMEM = {
      B00000,
      B01000,
      B01100,
      B11110,
      B11111,
      B11110,
      B01100,
      B01000};

  const byte enterChar[8] PROGMEM = {
      B00001,
      B00001,
      B00001,
      B00101,
      B01101,
      B11111,
      B01100,
      B00100};

  const byte exitChar[8] PROGMEM = {
      B00000,
      B00000,
      B10001,
      B01010,
      B00100,
      B01010,
      B10001,
      B00000};

  const byte bathTempChar[8] PROGMEM = {
      B00010,
      B00101,
      B00010,
      B11000,
      B10100,
      B11000,
      B10100,
      B11000};

  const byte tankTempChar[8] PROGMEM = {
      B00010,
      B00101,
      B00010,
      B00000,
      B11100,
      B01000,
      B01000,
      B01000};
  const byte veryLowBatteryChar[8] PROGMEM = {
      B01110,
      B10001,
      B10001,
      B10001,
      B10001,
      B10001,
      B10001,
      B11111};
  const byte lowBatteryChar[8] PROGMEM = {
      B01110,
      B10001,
      B10001,
      B10001,
      B10001,
      B11111,
      B11111,
      B11111};

  const byte midBatteryChar[8] PROGMEM = {
      B01110,
      B10001,
      B10001,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111};

  const byte fullBatteryChar[8] PROGMEM = {
      B01110,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111};

} // namespace Icons
