#pragma once

#include "Arduino.h"
#include <LCD_I2C.h>

// UI Pins
#define BUZZER 5
#define RED_LED 7
    // Motor Driver Pins 
// (Using Mini L298 Motor Driver Board)
#define MOT_IN1 8 // Agitate Motor 1
#define MOT_IN2 9 // Agitate Motor 2
#define MOT_IN3 10 // Vibrate Motor 1
#define MOT_IN4 11 // Vibrate Motor 2
#define LCD_ADDR 0x27 //Default LCD Address. Change for your module.

//Encoder Pins 
/* 
(Using Encoder Breakout Board with pullip resistors. 
If you dont have pullup resistors, you can enable the built in ones.
*/
#define ENC_CLK 2 // Encoder In B
#define ENC_DT 3 // Encoder In A
#define ENC_SW 4 // Encoder Button
#define LONG_PRESS_DUR 500 // Button long press duration

#define SENSOR_ERR -12.0f
#define SENSOR_NOT_READY -1.9f

namespace UI_Icons
{
  const byte leftArrowChar[] = {
    B00000,
    B00010,
    B00110,
    B01111,
    B11111,
    B01111,
    B00110,
    B00010
  };

  const byte rightArrowChar[] = {
    B00000,
    B01000,
    B01100,
    B11110,
    B11111,
    B11110,
    B01100,
    B01000
  };

  const byte enterChar[] = {
    B00001,
    B00001,
    B00001,
    B00101,
    B01101,
    B11111,
    B01100,
    B00100
  };

  const byte exitChar[] = {
    B00000,
    B00000,
    B10001,
    B01010,
    B00100,
    B01010,
    B10001,
    B00000
  };

  const byte bathTempChar[] = {
    B00010,
    B00101,
    B00010,
    B11000,
    B10100,
    B11000,
    B10100,
    B11000
  };

  const byte tankTempChar[] = {
    B00010,
    B00101,
    B00010,
    B00000,
    B11100,
    B01000,
    B01000,
    B01000
};

} // namespace UI_Icons
