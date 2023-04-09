#pragma once

#include <Arduino.h>
#include "globals.h"
#include "utilfuncs.h"
#include "tempSensorHelper.h"
#include "menuHelper.h"
#include "debugUtils.h"

//C-41 Process values
#define MAX_PULL -1 // Maximum stops that we can pull the film
#define MAX_PUSH 3 // Maximum stops that we can push the film

//Durations for the specific processes. Based on the values from the CineStill Cs-41 Kit.
#define PULL_ONE_DUR 165
#define STD_DEV_DUR 210
#define PUSH_ONE_DUR 273
#define PUSH_TWO_DUR 368
#define PUSH_THR_DUR 525
#define FIXING_TIME 480

namespace FilmDevHelpers
{
    /*
    --- agitate | Film Development Helper Functions ---
    duration: Agitate duration in seconds
    AGITATE_MOT_1: Motor Control Pin 1
    AGITATE_MOT_2: Motor Control Pin 2
    
    Runs the agitate motor for the amount of time provided, 
    each time in a different direction.
  */
  void agitate(int duration){
    DEBUG_PRINT("Agitation started.");

    bool agitateDirectionFlag = true;
    bool motorRunning = false;
    unsigned long timeNow;
       
    switch (agitateDirectionFlag)
    {
      case true:
        analogWrite(AGITATE_MOT_1, 255);
        analogWrite(AGITATE_MOT_2, 0);     
        agitateDirectionFlag = false;
        break;
      case false:
      default:
        analogWrite(AGITATE_MOT_1, 0);
        analogWrite(AGITATE_MOT_2, 255);
        agitateDirectionFlag = true;
        break;
    }

    timeNow = millis();
    motorRunning = true;

    while (motorRunning)
    {
      if (millis() > timeNow + (duration * 1000))
      {
        analogWrite(AGITATE_MOT_1, 0);
        analogWrite(AGITATE_MOT_2, 0);
        motorRunning = false;
      }
      TempSensors::requestTankTemp();
      MenuUI::printTempReadings(TempSensors::getTankTemp());
    }
    DEBUG_PRINT("Agitate Finished.")
    return;
  }

  /*
    --- vibrate | Film Development Helper Functions ---
    AGITATE_MOT_1: Motor Control Pin 1
    AGITATE_MOT_2: Motor Control Pin 2

    Simple vibrate function, used to release air bubbles from the emulsion surface.
  */
  void vibrate(){
    DEBUG_PRINT("Vibrate started.");
    unsigned long timeNow;
    bool motorRunning;

    for (int count = 0; count < 4; count++)
    {
      DEBUG_PRINT(String("Vibrated ") + count + String(" time(s)."));
      timeNow = millis();
      analogWrite(VIBRATE_MOT_1, 255);
      analogWrite(VIBRATE_MOT_2, 0);
      motorRunning = true;

      while (motorRunning)
      {
        if (millis() > timeNow + 1000)
        {
          analogWrite(VIBRATE_MOT_1, 0);
          analogWrite(VIBRATE_MOT_2, 0);
          motorRunning = false;
        }
        TempSensors::requestTankTemp();
        MenuUI::printTempReadings(TempSensors::getTankTemp());
      }
      timeNow = millis();
      while (!motorRunning)
      {
        if (millis() > timeNow + 500) motorRunning = true;
        TempSensors::requestTankTemp();
        MenuUI::printTempReadings(TempSensors::getTankTemp());
      }  
    }
    analogWrite(VIBRATE_MOT_2, 0);
    analogWrite(VIBRATE_MOT_1, 0);
    DEBUG_PRINT("Vibrate Ended");
    return;
  }

  /*
    --- develop | Film Development Functions ---
    Develop function that handles the development process.
  */
  void develop(uint16_t devDurationSeconds, 
                uint8_t firstAgitationDurationSeconds, 
                uint8_t agitationDurationSeconds, 
                uint16_t agitateEveryDurationSeconds){
    
    uint8_t totalCycles = (devDurationSeconds - firstAgitationDurationSeconds) / 
                          (agitationDurationSeconds + agitateEveryDurationSeconds);
    uint8_t padding = (devDurationSeconds - firstAgitationDurationSeconds) % 
                      (agitationDurationSeconds + agitateEveryDurationSeconds);    
    
    unsigned long timeNow;
    DEBUG_PRINT("develop(): started.");
    digitalWrite(RED_LED, HIGH);
    
    agitate(firstAgitationDurationSeconds);
    vibrate();

    DEBUG_PRINT("develop(): completed initial cycle.")
    for (uint8_t cycleCount = 0; cycleCount < totalCycles; cycleCount++)
    {
      DEBUG_PRINT(String("develop(): Cycle ") + cycleCount + String("started. "));
      
      timeNow = millis();

      while (millis() < timeNow + (agitateEveryDurationSeconds * 1000))
      {
        TempSensors::requestTankTemp();
        MenuUI::printTempReadings(TempSensors::getTankTemp());
        delay(250);
      }
      agitate(agitationDurationSeconds);
      DEBUG_PRINT("develop(): just agitated.")

      if (cycleCount + 2 >= totalCycles) Utils::buzz(3);
      
      timeNow = millis();
      while (millis() < timeNow + (padding * 1000))
      {
        TempSensors::requestTankTemp();
        MenuUI::printTempReadings(TempSensors::getTankTemp());
        delay(250);
      }
      Utils::buzz(5);
    }
    digitalWrite(RED_LED, LOW);
    DEBUG_PRINT("develop(): ending now.");
    return;
  }
  /*
    --- fix | Film Development Functions ---
    fix function handles the Fixing Step of the process. vales in seconds. 
    */
  void fix(uint8_t fixingDurationSeconds){
    uint8_t totalCycles = fixingDurationSeconds * 2;
    unsigned long timeNow;
    DEBUG_PRINT("fix(): started.");

    digitalWrite(RED_LED, HIGH);
    DEBUG_PRINT("develop(): calling develop now.");
    develop(FIXING_TIME, 10, 7, 30);

    Utils::buzz(6);
    digitalWrite(RED_LED, LOW);
    DEBUG_PRINT("develop(): ending now.");
    return;
    
  }
} // namespace FilmDevUtils
