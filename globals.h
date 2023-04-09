#pragma once

#include "Arduino.h"
#include <LCD_I2C.h>
#include "debugUtils.h"
#include "encoderHelper.h"


#define RED_LED 7

// Motor Driver Pins 
// (Using Mini L298 Motor Driver Board)
#define AGITATE_MOT_1 8 // Agitate Motor 1
#define AGITATE_MOT_2 9 // Agitate Motor 2
#define VIBRATE_MOT_1 10 // Vibrate Motor 1
#define VIBRATE_MOT_2 11 // Vibrate Motor 2

enum class machineState : uint8_t {
  IDLE,   
  DEVELOPING,
  MONITORING
};

machineState currState = machineState::IDLE;

void setState(machineState newState){
  DEBUG_PRINT("State Was: ");
  #ifdef DEBUG
  switch (currState)
    {
    case machineState::IDLE:
      DEBUG_PRINT("IDLE");
      break;
    case machineState::DEVELOPING:
      DEBUG_PRINT("DEVELOPING");
      break;
    case machineState::MONITORING:
      DEBUG_PRINT("MONITORING");
      break;
    default:
      DEBUG_PRINT("NULL");
      DEBUG_TRACE();
      break;
    }
  #endif
  if (currState != machineState::IDLE && newState == machineState::IDLE)
  { 
    MenuUI::refreshMenu();
  }
  currState = newState;
  DEBUG_PRINT("State became: ");
  #ifdef DEBUG
    switch (currState)
    {
    case machineState::IDLE:
      DEBUG_PRINT("IDLE");
      break;
    case machineState::DEVELOPING:
      DEBUG_PRINT("DEVELOPING");
      break;
    case machineState::MONITORING:
      DEBUG_PRINT("MONITORING");
      break;
    default:
      DEBUG_PRINT("NULL");
      DEBUG_TRACE();
      break;
    }
  #endif

  return;
}