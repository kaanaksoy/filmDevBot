#ifndef DEVELOP_FILM_HPP
// Define the header guard symbol to prevent multiple inclusion
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

// This header guards against multiple inclusion of the same file.

namespace DevelopFilm
{
  // Strings for different film types and states
  const char p_C41[] PROGMEM = "C-41";
  const char p_E6[] PROGMEM = "E-6";
  const char p_BW[] PROGMEM = "B&W";
  const char p_Custom[] PROGMEM = "Custom";

  const char p_psh[] PROGMEM = "Psh";
  const char p_pll[] PROGMEM = "Pll";

  const char p_toStrt[] PROGMEM = "To Start";
  const char p_running[] PROGMEM = "Running";
  const char p_finsihed[] PROGMEM = "Finished";
  const char p_fixing[] PROGMEM = "Fix";
  const char p_dev[] PROGMEM = "Dev";
  const char p_toExit[] PROGMEM = "To Exit";
  const char p_Params[] PROGMEM = "To Enter";
  const char p_Dur[] PROGMEM = "Dur";
  const char p_Agit[] PROGMEM = "Agit";
  const char p_fst[] PROGMEM = "Frst";
  const char p_every[] PROGMEM = "Intrvl";
  const char p_secs[] PROGMEM = "Secs";

  /* -------------------------------------------------------------------------- */
  /*              Function prototypes for the DevelopFilm namespace             */
  /* -------------------------------------------------------------------------- */

  // ColorC41 function for color film development
  void
  ColorC41();

  // Custom function for custom film development settings
  void Custom();

  /*
      Agitate helper function for film development
      duration: Agitate duration in seconds
  */
  int agitate(uint16_t duration = 0);

  // Develop function for the film development process
  int develop(int devDurSec = 0, int fstAgitDurSec = 0, int agitDurSec = 0,
              int agitEvryDurSec = 0, int fixDurSec = 0, int fstFixAgitDurSec = 0,
              int fixAgitDurSec = 0, int fixAgitEveryDurSec = 0);

  // StartMonitor function to initiate temperature monitoring
  void StartMonitor();

  // Monitoring function to monitor temperature and control indicators
  void Monitoring(int tempReading, IndicatorParamType action = IndicatorParamType::TOGGLE);

} // namespace DevelopFilm

#endif