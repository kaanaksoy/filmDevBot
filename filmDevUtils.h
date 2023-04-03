#pragma once

#include <Arduino.h>
#include "globals.h"
#include "utilfuncs.h"

namespace filmDevUtils
{
    /*
    --- agitate | Film Development Helper Functions ---
    dur: Agitate duration in seconds
    motorPin1: Motor Control Pin 1
    motorPin2: Motor Control Pin 2
    
    Runs the agitate motor for the amount of time provided, 
    each time in a different direction.
  */
  void agitate(int dur, int motorPin1, int motorPin2){
    bool agitateDirectionFlag = true;
    unsigned long timeNow;

    switch (agitateDirectionFlag)
    {
    case true:
      analogWrite(motorPin1, 255);
      analogWrite(motorPin2, 0);

      timeNow = millis();
      Utils::millisDelay(timeNow, dur*1000);

      agitateDirectionFlag = false;
      break;
    case false:
    default:
      analogWrite(motorPin1, 0);
      analogWrite(motorPin2, 255);

      timeNow = millis();
      Utils::millisDelay(timeNow, dur*1000);

      agitateDirectionFlag = true;
      break;
    }

    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, 0);
    
    return;
  }

  /*
    --- vibrate | Film Development Helper Functions ---
    motorPin1: Motor Control Pin 1
    motorPin2: Motor Control Pin 2

    Simple vibrate function, used to release air bubbles from the emulsion surface.
  */
  void vibrate(int motorPin1, int motorPin2){
    unsigned long timeNow;

    for (int i = 0; i < 4; i++)
    {
    analogWrite(motorPin1, 255);
    analogWrite(motorPin2, 0);
    timeNow = millis();
    Utils::millisDelay(timeNow, 1000);
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, 0);
    timeNow = millis();
    Utils::millisDelay(timeNow, 500);
    }
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
    
    uint8_t totalCycles = (devDurationSeconds - firstAgitationDurationSeconds) / (agitationDurationSeconds + agitateEveryDurationSeconds);
    uint8_t padding = (devDurationSeconds - firstAgitationDurationSeconds) % (agitationDurationSeconds + agitateEveryDurationSeconds);    
    
    unsigned long timeNow;

    digitalWrite(RED_LED, HIGH);
    agitate(firstAgitationDurationSeconds, MOT_IN1, MOT_IN2);
    vibrate(MOT_IN3, MOT_IN4);
    for (uint8_t cycleCount = 0; cycleCount < totalCycles; cycleCount++)
    {
      timeNow = millis();
      Utils::millisDelay(timeNow, agitateEveryDurationSeconds*1000);
      agitate(agitationDurationSeconds, MOT_IN1, MOT_IN2);
      if (cycleCount + 2 >= totalCycles)
      {
        Utils::buzz(3);
      }
      timeNow = millis();
      Utils::millisDelay(timeNow, padding*1000);
      Utils::buzz(5);
      
    }
    digitalWrite(RED_LED, LOW);

    return;
  }
  /*
    --- fix | Film Development Functions ---
    fix function handles the Fixing Step of the process. vales in seconds. 
    */
  void fix(uint8_t fixingDurationSeconds){
    uint8_t totalCycles = fixingDurationSeconds * 2;
    unsigned long timeNow;
    digitalWrite(RED_LED, HIGH);
    vibrate(MOT_IN3, MOT_IN4);
    for (uint8_t cycleCount = 0; cycleCount < totalCycles; cycleCount++)
    {
      agitate(15, MOT_IN1, MOT_IN2);
      timeNow = millis();
      Utils::millisDelay(timeNow, 15000);
    }
    Utils::buzz(6);
    digitalWrite(RED_LED, LOW);
    return;
    
  }
} // namespace FilmDevUtils
