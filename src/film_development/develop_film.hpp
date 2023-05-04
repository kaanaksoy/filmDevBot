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

    void Custom();

    /*
        --- agitate | Film Development Helper Functions ---
        duration: Agitate duration in seconds
        AGITATE_MOT_1: Motor Control Pin 1
        AGITATE_MOT_2: Motor Control Pin 2

        Runs the agitate motor for the amount of time provided,
        each time in a different direction.
    */
    int agitate(uint16_t duration = 0);

    /*
      --- develop | Film Development Functions ---
      Develop function that handles the development process.
    */
    // int develop(uint16_t devDurSec = 0, uint8_t fstAgitDurSec = 0, uint8_t agitDurSec = 0,
    //             uint16_t agitEvryDurSec = 0, uint16_t fixDurSec = 0, uint8_t fstFixAgitDurSec = 0,
    //             uint8_t fixAgitDurSec = 0, uint16_t fixAgitEveryDurSec = 0);
    int develop(int devDurSec = 0, int fstAgitDurSec = 0, int agitDurSec =0,
                int agitEvryDurSec = 0, int fixDurSec = 0, int fstFixAgitDurSec = 0,
                int fixAgitDurSec = 0, int fixAgitEveryDurSec = 0);

    void fix(uint8_t fixingDurationSeconds);

} // namespace DevelopFilm

#endif