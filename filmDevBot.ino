//#define DEBUG // Uncomment to turn on debug statements.
#include <ArduinoTrace.h>
#include "CMBMenu.hpp"
#include <LCD_I2C.h>

#include "filmDevUtils.h" // Helper functions for film development
#include "globals.h" // Global variables
#include "encoderHelper.h"
#include "debugUtils.h"
#include "menuHelper.h"
#include "utilfuncs.h"
#include "tempSensorHelper.h"


/*
  Film Development Bot
  - Automates your film development using AP Tanks. 
*/
unsigned long timeEncoderPolled = millis();
enc::EncoderInputType command = enc::EncoderNone;
enc::EncoderInputType prevCommand;

// ------------ SETUP -------------
void setup(){
 
  
  #ifdef DEBUG
  Serial.begin(9600);
  #endif


  MenuUI::initLCD();
  MenuUI::createMenu();
  TempSensors::initializeTempSensor();
  
  // Initialize motor control pins.
  pinMode(MOT_IN1, OUTPUT);
  pinMode(MOT_IN2, OUTPUT);
  pinMode(MOT_IN3, OUTPUT);
  pinMode(MOT_IN4, OUTPUT);

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

  enc::EncoderInputType command = enc::getCommand(command);
  if (command == enc::EncoderExit) delay(400);

  // if (millis() - timeEncoderPolled > 200)
  // {
  //   timeEncoderPolled = millis();
  //    // Determine pressed command.
    
  // }
  

  // Call menu methods based on command selection
  switch (command)
  {
  case enc::EncoderExit:
    MenuUI::gMenu.exit();
    break;
  case enc::EncoderEnter:
    MenuUI::gMenu.enter(layerChanged);
    break;
  case enc::EncoderLeft:
    MenuUI::gMenu.left();
    break;
  case enc::EncoderRight:
    MenuUI::gMenu.right();
  case enc::EncoderNone:
  default:
    break;
  }

  /* 
    Print/update the menu when commanded.
    get the current function ID.
  */
  if (enc::EncoderNone != command)
  {
    fid = MenuUI::gMenu.getInfo(info);
    MenuUI::printMenuEntry(info);

    // Do action regarding fid 
    if ((0 != fid) && (command == enc::EncoderEnter) && (!layerChanged)){
      switch (fid)
      {
      case MenuUI::MenuC41:
        ColorC41();
        break;
      case MenuUI::MenuE6:
        ColorE6();
        break;
      case MenuUI::MenuBWCustom:
        BWCustom();
        break;
      default:
        break;
      }
    }
  }
  TempSensors::requestTankTemp();
  MenuUI::printTempReadings(TempSensors::getTankTemp());
  


}

// --- ColorC41 | Film Development Functions ---
void ColorC41(){

  int8_t pushPullValue = 0;
  bool confirmSelection = false;
  uint16_t duration;

  gLCD.clear();
  gLCD.setCursor(0,0);
  gLCD.println("C-41 Development");


  while (!confirmSelection)
  {
    enc::EncoderInputType command = enc::getCommand(command); // Determine pressed command.
    
    switch (command)
    {
    case enc::EncoderLeft:
      pushPullValue--;
      break;
    case enc::EncoderRight:
      pushPullValue++;
      break;
    case enc::EncoderExit:
      return;
    case enc::EncoderEnter:
      confirmSelection = true;
    default:
      break;
    }
    if (pushPullValue > 3)
    {
      pushPullValue = 3;
    }
    else if (pushPullValue < -1)
    {
      pushPullValue = -1;
    }
    gLCD.setCursor(0,1);
    gLCD.println(String("Push/Pull: ") + String(pushPullValue) + String(" stp"));
    delay(150);
  }
  confirmSelection = false;
  Utils::buzz(abs(pushPullValue));
  gLCD.clear(); 
  gLCD.setCursor(0,0);

  switch (pushPullValue)
  {
  case 0:
    duration = 210;
    gLCD.print("C-41, +0 Push");
    break;
  case 1:
    duration = 273;
    gLCD.print("C-41, +1 Push");
    break;
  case 2:
    duration = 368;
    gLCD.print("C-41, +2 Push");
    break;
  case 3:
    duration = 525;
    gLCD.print("C-41, +3 Push");
    break;
  case -1:
    duration = 165;
    gLCD.print("C-41, -1 Pull");
  default:
    break;
  }

  gLCD.setCursor(0,1);
  gLCD.createChar(2, UI_Icons::enterChar);
  gLCD.setCursor(3,1);
  gLCD.write(2);
  gLCD.setCursor(5, 1);
  gLCD.print("to start...");

  while (!confirmSelection)
  {
    enc::EncoderInputType command = enc::getCommand(command);
    switch (command)
    {
    case enc::EncoderEnter:
      confirmSelection = true;
      break;
    case enc::EncoderExit:
      return;
    default:
      break;
    }
    Utils::buzz(1);
  }
  confirmSelection = false;
  
  gLCD.clear();
  gLCD.print("Developing...");
  filmDevUtils::develop(duration, 10, 6, 30);

  gLCD.clear();
  gLCD.print("Development Finished");
  gLCD.setCursor(0,1);
  gLCD.createChar(2, UI_Icons::enterChar);
  gLCD.setCursor(3,1);
  gLCD.write(2);
  gLCD.setCursor(5, 1);
  gLCD.print("to start fixing...");
  gLCD.clear();

  while (!confirmSelection)
  {
    enc::EncoderInputType command = enc::getCommand(command);
    switch (command)
    {
    case enc::EncoderEnter:
      confirmSelection = true;
      break;
    case enc::EncoderExit:
      return;
    default:
      break;
    }
    Utils::buzz(1);
  }
  confirmSelection = false;

  gLCD.print("Fixing...");
  filmDevUtils::fix(480);

  gLCD.clear();
  gLCD.setCursor(0,0);
  gLCD.print("Process Finished");
  gLCD.setCursor(0,1);
  gLCD.createChar(2, UI_Icons::enterChar);
  gLCD.setCursor(3,1);
  gLCD.write(2);
  gLCD.setCursor(5, 1);
  gLCD.print("to start...");
  gLCD.clear();

  while (!confirmSelection)
  {
    enc::EncoderInputType command = enc::getCommand(command);
    switch (command)
    {
    case enc::EncoderEnter:
      confirmSelection = true;
      break;
    case enc::EncoderExit:
      return;
    default:
      break;
    }
    Utils::buzz(1);
  }
  confirmSelection = false;
  return;
}

void ColorE6(){
  return;
}

void BWCustom(){
  return;
}


