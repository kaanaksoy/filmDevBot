#ifndef DEVELOP_FILM_HPP
#define DEVELOP_FILM_HPP
#pragma once

#include "Arduino.h"
#include "../../globals.h"
#include "../../debugUtils.h"
#include "../resources/icons.hpp"
#include "../interface/encoder_helper.hpp"
#include "../interface/led_helper.hpp"
#include "../utilities/state_manager.hpp"
#include "../interface/menu_helper.hpp"
#include "../utilities/general_utilities.hpp"
#include "../sensors/temp_sensor_helper.hpp"

namespace DevelopFilm
{
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