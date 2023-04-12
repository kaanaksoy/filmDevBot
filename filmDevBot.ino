/*
  Film Development Bot
  - Automates your film development using AP Tanks.
*/

// #define DEBUG // Uncomment to turn on debug statements.
#include <ArduinoTrace.h>
#include <LCD_I2C.h>

/* --------------------------------- Headers -------------------------------- */

#include "globals.h" // Global variables

#include "src/utilities/state_manager.hpp"
#include "src/interface/encoder_helper.hpp"
#include "debugUtils.h"
#include "src/interface/menu_helper.hpp"
#include "src/interface/led_helper.hpp"
#include "src/utilities/general_utilities.hpp"
#include "src/sensors/temp_sensor_helper.hpp"
#include "src/film_development/develop_film.hpp"
#include "src/utilities/battery_utilities.hpp"

/* ---------------------------------- SETUP --------------------------------- */
void setup()
{

#ifdef DEBUG
  Serial.begin(9600);
#endif

  StateManager::State.currentMillis = millis();
  SystemEncoder::EncoderInputType command = SystemEncoder::EncoderNone;

  BatteryMonitor::initBatteryChargeMeasurement();
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
  pinMode(RED_LED_PIN, OUTPUT);
  // Initialize encoder button.
  pinMode(ENC_SW, INPUT);
}

/* ---------------------------------- LOOP ---------------------------------- */

void loop()
{

  StateManager::State.currentMillis = millis();

  if (StateManager::State.ledInUse)
    StatusLED::blink();

  int fid = 0; // Function ID

  // Info text from menu
  const char *info;
  bool layerChanged = false; // Should navigate layers?

  switch (StateManager::State.currentState)
  {
  case StateManager::IDLE:
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
      if ((0 != fid) && (command == SystemEncoder::EncoderEnter) && (!layerChanged))
      {
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

  case StateManager::DEVELOPING:

    TempSensors::requestTankTemp();
    MenuUI::printTempReadings(TempSensors::getTankTemp());

    break;

  case StateManager::MONITORING:
    setState(StateManager::IDLE);
  default:
    break;
  }
}
