#ifndef GLOBALS_H
#define GLOBALS_H

/* -------------------------------------------------------------------------- */
/*                             Global constants                             */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/* ---- All of the global constants, variables & pin definitions are here --- */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                       Constants for C-41 Development                       */
/* -------------------------------------------------------------------------- */

// Maximum stops that we can pull the film
#define MAX_PULL -1
// Maximum stops that we can push the film
#define MAX_PUSH 3

// Durations for the specific processes.
// Based on the values from the CineStill Cs-41 Kit.
#define PULL_ONE_DEV_DUR 165
#define STD_DEV_DUR 210
#define PUSH_ONE_DEV_DUR 273
#define PUSH_TWO_DEV_DUR 368
#define PUSH_THR_DEV_DUR 525
#define FIXING_DUR 480

/* -------------------------------------------------------------------------- */
/*                             Constants Interface                            */
/* -------------------------------------------------------------------------- */

#define BUZZER 5
#define RED_LED_PIN 7
// Adjust to change how fast the LED blinks.
#define BLINK_PERIOD 1000

/* ---------------------- RGB LED with FastLED Library ---------------------- */

// Number of LEDs connected in series
#define NUM_LEDS 1
#define RGB_LED_PIN 12
// (0-255) Sets max brightness
#define STATUS_LED_BRIGHTNESS 50

// Default LCD Address that matches my unit. Change for your module.
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

/* ------------------------------ Motor Control ----------------------------- */
/* Motor Driver Pins, based on the mini L298 motor driver, change to suit your
    needs.
*/

#define AGITATE_MOT_1 8
#define AGITATE_MOT_2 9
#define VIBRATE_MOT_1 10
#define VIBRATE_MOT_2 11

// The PWM value for the vibrate motor, use it to set your vibrate motor speed (0 - 255)
#define VIBRATE_PWM
// The PWM value for the agitate motor, use it to set your agitate motor speed (0 - 255)
#define AGITATE_PWM

/* --------------------------------- Encoder -------------------------------- */

// Encoder In A
#define ENC_DT 3
// Encoder In B
#define ENC_CLK 2
// Encoder Button
#define ENC_SW 4
// Encoder button long press duration
#define LONG_PRESS_DUR 500

/* --------------------------------- Sensors -------------------------------- */

// Data wire is plugged into port 6 on the Arduino
#define ONE_WIRE_BUS_PIN 6

/* -------------------------------------------------------------------------- */
/*                         Battery Charge Measurement                         */
/* -------------------------------------------------------------------------- */

#define BATTERY_SENSE_PIN A0

// Adjust this constant to change how often battery is checked (in minutes).
#define BATT_CHECK_PERIOD 2

/* The following thresholds are based on experimentation with the specific
    ATMEGA Chip used in development. They may be inaccurate in other instances.

    In my case the values are best defined by the function:
        voltageRead = 212.365(batteryVoltage) + 140.332
*/
#define FULL_CHARGE_THRESHOLD 950
#define MID_CHARGE_THRESHOLD 870
#define LOW_CHARGE_THRESHOLD 780

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                              Global variables                              */
/* -------------------------------------------------------------------------- */

#endif