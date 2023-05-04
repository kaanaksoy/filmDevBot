/*
  Film Development Bot
  - Automates your film development using AP Tanks.
*/

//#define DEBUG // Uncomment to turn on debug statements.
#include <ArduinoTrace.h>
#include <LCD_I2C.h>

/* --------------------------------- Headers -------------------------------- */

#include "globals.h" // Global variables
#include "src/utilities/state_manager.hpp"
#include "src/interface/encoder_helper.hpp"
#include "debugUtils.h"
#include "src/interface/menu_helper.hpp"
#include "src/interface/indicator_helpers.hpp"
#include "src/interface/indicator_helpers.hpp"
#include "src/sensors/temp_sensor_helper.hpp"
#include "src/film_development/develop_film.hpp"
#include "src/utilities/battery_utilities.hpp"
#include "src/interface/lcd_helper.hpp"

/* ---------------------------------- SETUP --------------------------------- */
StateType State = {
    OperationStateType::IDLE,
    IndicatorStateType::AVAILABLE,
    IndicatorStateType::AVAILABLE,
    ChargeLevelType::FullCharge,
    millis(),
};

char tmpStr[STR_BUFF_LEN] = "";

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
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  // Initialize encoder button.
  pinMode(ENC_SW, INPUT);
}

/* ---------------------------------- LOOP ---------------------------------- */

void loop()
{
  State.currentMillis = millis();
  BatteryMonitor::updateChargeLevel(State.batteryLevel);

  if (State.ledState == IndicatorStateType::BUSY)
    Indicators::blinkLEDs();
  if (State.buzzerState == IndicatorStateType::BUSY)
    Indicators::buzz();

  switch (State.currentState)
  {
  case OperationStateType::DEVELOPING:
    DevelopFilm::develop();
    TempSensors::requestTankTemp();
    MenuUI::printTempReadings(TempSensors::getTankTemp());
    break;

  case OperationStateType::INDEVELOPMENU:
    TempSensors::requestTankTemp();
    MenuUI::printTempReadings(TempSensors::getTankTemp());
    break;

  case OperationStateType::IDLE:
  
    int fid = 0; // Function ID

    // Info text from menu
    const char *info;
    bool layerChanged = false; // Should navigate layers?
    EncoderInputType command = SystemEncoder::getCommand(command);
    TempSensors::requestTankTemp();
    MenuUI::printTempReadings(TempSensors::getTankTemp());
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
        DEBUG_PRINT(fid);
        switch (fid)
        {
        case MenuUI::MenuC41:
          DEBUG_TRACE();
          DevelopFilm::ColorC41();
          break;
        case MenuUI::MenuCustom:
          DevelopFilm::Custom();
          break;
        default:
          break;
        }
      }
    }
    TempSensors::requestTankTemp();
    MenuUI::printTempReadings(TempSensors::getTankTemp());
  break;

  case OperationStateType::MONITORING:
    StateManager::setOperationState(IDLE);
  default:
    break;
  }
}
