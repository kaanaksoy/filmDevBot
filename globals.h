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

// create LCD Instance
LCD_I2C gLCD(0x27, 16, 2); //Default LCD Address. Change for your module.

enum EncoderInputType {
EncoderNone,
EncoderLeft,
EncoderRight,
EncoderEnter,
EncoderExit
};


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
} // namespace UI_Icons
