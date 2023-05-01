#ifndef DEVELOP_FILM_HPP
#define DEVELOP_FILM_HPP
#pragma once

#include "Arduino.h"
#include "../../globals.h"
#include "../../debugUtils.h"
#include "../resources/icons.hpp"
#include "../interface/encoder_helper.hpp"
#include "../interface/indicator_helpers.hpp"
#include "../interface/lcd_helper.hpp"
#include "../utilities/state_manager.hpp"
#include "../interface/menu_helper.hpp"
#include "../interface/indicator_helpers.hpp"
#include "../sensors/temp_sensor_helper.hpp"

namespace DevelopFilm
{
  // Strings
  const char p_C41[] PROGMEM = "C-41";
  const char p_E6[] PROGMEM = "E-6";
  const char p_BW[] PROGMEM = "B&W";
  const char p_Custom[] PROGMEM = "Custom";

  const char p_psh[] PROGMEM = "Psh";
  const char p_pll[] PROGMEM = "Pll";

  const char p_toStrt[] PROGMEM = "To Start";
  const char p_running[] PROGMEM = "Running";
  const char p_finsihed[] PROGMEM = "Finished";
  const char p_fixing[] PROGMEM = "Fixing";
  const char p_dev[] PROGMEM = "Dev";
  const char p_toExit[] PROGMEM = "To Exit";

  // --- ColorC41 | Film Development Functions ---
  void ColorC41();

  void ColorE6();

  void BWCustom();

  /*
      --- agitate | Film Development Helper Functions ---
      duration: Agitate duration in seconds
      AGITATE_MOT_1: Motor Control Pin 1
      AGITATE_MOT_2: Motor Control Pin 2

      Runs the agitate motor for the amount of time provided,
      each time in a different direction.
  */
  void agitate(int duration);

  /*
    --- vibrate | Film Development Helper Functions ---
    AGITATE_MOT_1: Motor Control Pin 1
    AGITATE_MOT_2: Motor Control Pin 2

    Simple vibrate function, used to release air bubbles from the emulsion surface.
  */
  void vibrate();

  /*
    --- develop | Film Development Functions ---
    Develop function that handles the development process.
  */
  void develop(uint16_t devDurationSeconds,
               uint8_t firstAgitationDurationSeconds,
               uint8_t agitationDurationSeconds,
               uint16_t agitateEveryDurationSeconds);

  void fix(uint8_t fixingDurationSeconds);

} // namespace DevelopFilm

#endif