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
#include "src/interface/lcd_helper.hpp"

StateType State = {
    OperationStateType::IDLE,
    false,
    false,
    millis(),
    ChargeLevelType::FullCharge};

/* ---------------------------------- SETUP --------------------------------- */
void setup()
{

#ifdef DEBUG
  Serial.begin(9600);
#endif

  Display::initDisplay();
  MenuUI::createMenu();
  SystemEncoder::initEncoder();
  EncoderInputType command = EncoderNone;
  BatteryMonitor::initBatteryChargeMeasurement();
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

  State.currentMillis = millis();
  BatteryMonitor::updateChargeLevel(State.batteryLevel);

  if (State.ledInUse)
    StatusLED::blink();

  int fid = 0; // Function ID

  // Info text from menu
  const char *info;
  bool layerChanged = false; // Should navigate layers?

  switch (State.currentState)
  {
  case IDLE:
    EncoderInputType command = SystemEncoder::getCommand(command);
    // Call menu methods based on command selection
    switch (command)
    {
    case EncoderExit:
      MenuUI::gMenu.exit();
      break;
    case EncoderEnter:
      MenuUI::gMenu.enter(layerChanged);
      break;
    case EncoderLeft:
      MenuUI::gMenu.left();
      break;
    case EncoderRight:
      MenuUI::gMenu.right();
    case EncoderNone:
    default:
      break;
    }

    /*
      Print/update the menu when commanded.
      get the current function ID.
    */
    if (EncoderNone != command)
    {
      fid = MenuUI::gMenu.getInfo(info);
      MenuUI::printMenuEntry(info);

      // Do action regarding fid
      if ((0 != fid) && (command == EncoderEnter) && (!layerChanged))
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

  case DEVELOPING:

    TempSensors::requestTankTemp();
    MenuUI::printTempReadings(TempSensors::getTankTemp());

    break;

  case MONITORING:
    StateManager::setOperationState(IDLE);
  default:
    break;
  }
}
