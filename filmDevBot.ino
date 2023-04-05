//#define DEBUG // Uncomment to turn on debug statements.
#include <ArduinoTrace.h>
#include "CMBMenu.hpp"
#include <LCD_I2C.h>

#include "globals.h" // Global variables
#include "encoderHelper.h"
#include "debugUtils.h"
#include "menuHelper.h"
#include "utilfuncs.h"
#include "tempSensorHelper.h"
#include "filmDev.h"
#include "filmDevUtils.h" // Helper functions for film development


/*
  Film Development Bot
  - Automates your film development using AP Tanks. 
*/

// ------------ SETUP -------------
void setup(){
 
  #ifdef DEBUG
  Serial.begin(9600);
  #endif

  SystemEncoder::EncoderInputType command = SystemEncoder::EncoderNone;
  machineState currState = machineState::IDLE;

  MenuUI::initLCD();
  MenuUI::createMenu();
  TempSensors::initializeTempSensor();
  
  // Initialize motor control pins.
  pinMode(AGITATE_MOT_1, OUTPUT);
  pinMode(AGITATE_MOT_2, OUTPUT);
  pinMode(VIBRATE_MOT_1, OUTPUT);
  pinMode(VIBRATE_MOT_2, OUTPUT);

  // Initialize UI pins.
  pinMode(BUZZER, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  // Initialize encoder button.
  pinMode(ENC_SW, INPUT);

  
}

// ------------ LOOP -------------
void loop(){
  int fid = 0; //Function ID

  // Info text from menu
  const char* info;
  bool layerChanged = false; // Should navigate layers?

  switch (currState)
  {
  case machineState::IDLE:  
    SystemEncoder::EncoderInputType command = SystemEncoder::getCommand(command);  
    // Call menu methods based on command selection
    switch (command)
    {
    case SystemEncoder::EncoderExit:
      MenuUI::gMenu.exit();
      break;
    case SystemEncoder::EncoderEnter:
      MenuUI::gMenu.enter(layerChanged);
      break;
    case SystemEncoder::EncoderLeft:
      MenuUI::gMenu.left();
      break;
    case SystemEncoder::EncoderRight:
      MenuUI::gMenu.right();
    case SystemEncoder::EncoderNone:
    default:
      break;
    }

    /* 
      Print/update the menu when commanded.
      get the current function ID.
    */
    if (SystemEncoder::EncoderNone != command)
    {
      fid = MenuUI::gMenu.getInfo(info);
      MenuUI::printMenuEntry(info);

      // Do action regarding fid 
      if ((0 != fid) && (command == SystemEncoder::EncoderEnter) && (!layerChanged)){
        switch (fid)
        {
        case MenuUI::MenuC41:
          DevelopFilm::ColorC41();
          break;
        case MenuUI::MenuE6:
          DevelopFilm::ColorE6();
          break;
        case MenuUI::MenuBWCustom:
          DevelopFilm::BWCustom();
          break;
        default:
          break;
        }
      }
    }
    TempSensors::requestTankTemp();
    MenuUI::printTempReadings(TempSensors::getTankTemp());
  
    break;


  case machineState::DEVELOPING:

    TempSensors::requestTankTemp();
    MenuUI::printTempReadings(TempSensors::getTankTemp());

    break;


  case machineState::MONITORING:
    currState = machineState::IDLE;
  default:
    break;
  }


}


