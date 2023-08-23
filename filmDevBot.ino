/*
  Film Development Bot
  - Automates your film development using AP Tanks.
*/

#include <ArduinoTrace.h>
#include <LCD_I2C.h>

#define DEBUG // Uncomment to turn on debug statements.
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
    2200,
};

EncoderInputType command;

char tmpStr[STR_BUFF_LEN];

bool shutDownRequestReceived = false; // Flag for Shutdown ISR
bool redrawMenu = true;
unsigned long battMillis;
unsigned long tempMillis;

void setup()
{

#ifdef DEBUG
  Serial.begin(9600);
#endif
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
  // Communication bus for power state.
  pinMode(KILL_BUS, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(KILL_BUS), shutDownISR, FALLING);

  SystemEncoder::initEncoder();
  BatteryMonitor::initBatteryChargeMeasurement();
  TempSensors::initializeTempSensor();
  Indicators::initLEDs();
  battMillis = millis();
  tempMillis = battMillis;

  command = EncoderNone;
  Display::initDisplay();
  MenuUI::createMenu();
}

/* ---------------------------------- LOOP ---------------------------------- */

void loop()
{
  if (shutDownRequestReceived)
  {
    Serial.println("Power Off Requested.");
    if (State.currentState != DEVELOPING)
    {
      Serial.println("Shutting Down");
      detachInterrupt(digitalPinToInterrupt(KILL_BUS));
      delay(1000);
      digitalWrite(KILL_BUS, LOW);
      pinMode(KILL_BUS, OUTPUT);
      Serial.println("KILL confirmation sent");
    }
    else
    {
      Serial.println("Shutdown Refused");
    }
    shutDownRequestReceived = false;
  }

  // Do housekeeping tasks
  State.currentMillis = millis();

  if (redrawMenu)
  {
    MenuUI::printBatteryInfo();
    MenuUI::printTempReadings(TempSensors::getTankTemp());
    redrawMenu = false;
  }

  if (State.currentMillis - tempMillis >= 500)
  {
    TempSensors::requestTankTemp();
    MenuUI::printTempReadings(TempSensors::getTankTemp());
    tempMillis = State.currentMillis;
  }
  if (State.currentMillis - battMillis >= 5000)
  {
    BatteryMonitor::updateChargeLevel(State.batteryLevel);
    MenuUI::printBatteryInfo();
    battMillis = State.currentMillis;
  }

  if (State.ledState == IndicatorStateType::BUSY)
    Indicators::blinkLEDs();

  if (State.buzzerState == IndicatorStateType::BUSY)
    Indicators::buzz();

  // Do State related action
  switch (State.currentState)
  {
    /* ---------------------------- Developing State ---------------------------- */
  case OperationStateType::DEVELOPING:
    DevelopFilm::develop();
    break;
    /* --------------------------- Develop Menu State --------------------------- */
  case OperationStateType::INDEVELOPMENU:
    // TempSensors::requestTankTemp();
    // MenuUI::printTempReadings(TempSensors::getTankTemp());
    break;
    /* ---------------------------- Monitoring State ---------------------------- */
  case OperationStateType::MONITORING:
    if (TempSensors::getTankTemp() != SENSOR_NOT_READY)
    {
      DevelopFilm::Monitoring(int(TempSensors::getTankTemp() * 100));
    }

    // command = EncoderNone;
    command = SystemEncoder::getCommand(command);
    if (command == EncoderExit)
    {
      DevelopFilm::Monitoring(State.setTemp, IndicatorParamType::STOP);
      StateManager::setOperationState(IDLE);
    }
    break;
    /* ------------------------------- IDLE STATE ------------------------------- */
  case OperationStateType::IDLE:
    command = SystemEncoder::getCommand(command);
    // TempSensors::requestTankTemp();
    // MenuUI::printTempReadings(TempSensors::getTankTemp());

    int fid = 0; // Function ID
    // Info text from menu
    const char *info;
    bool layerChanged = false; // Should navigate layers?
    // Call menu methods based on command selection
    if (command != EncoderNone)
    {
      redrawMenu = true;
    }

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
          DevelopFilm::ColorC41();
          break;
        case MenuUI::MenuCustom:
          DevelopFilm::Custom();
          break;
        case MenuUI::MenuMonitor:
          DevelopFilm::StartMonitor();
        default:
          break;
        }
      }
    }
    // TempSensors::requestTankTemp();
    // MenuUI::printTempReadings(TempSensors::getTankTemp());
    break;

  default:
    break;
  }
}

void shutDownISR()
{
  Serial.println("ShutdownISR");
  shutDownRequestReceived = true;
}
