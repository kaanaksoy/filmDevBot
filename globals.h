/* This header file contains various constants, pin definitions, enums, and
    global variables used throughout the film development machine's codebase. */

#ifndef GLOBALS_H
#define GLOBALS_H
#include <Arduino.h>

// This header guards against multiple inclusion of the same file.

/* -------------------------------------------------------------------------- */
/*                             Global constants                             */
/* -------------------------------------------------------------------------- */

// Buffer length for temporary strings
#define STR_BUFF_LEN 16

// Macro to count the number of elements in an array
#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

/* -------------------------------------------------------------------------- */
/*                       Constants for C-41 Development                       */
/* -------------------------------------------------------------------------- */

// Maximum stops that we can pull the film
#define MAX_PULL -1
// Maximum stops that we can push the film
#define MAX_PUSH 3

// Durations for specific processes in film development
#define PULL_ONE_DEV_DUR 165
#define STD_DEV_DUR 210
#define PUSH_ONE_DEV_DUR 273
#define PUSH_TWO_DEV_DUR 368
#define PUSH_THR_DEV_DUR 525

// Various durations for agitation and fixing
#define STD_FRST_AGITATE_DUR 10
#define AGITATE_DUR 10
#define STD_AGITATE_EVERY_DUR 30
#define FIXING_DUR 480

// Temperature limits for monitoring set temperature
#define MONITOR_SET_TEMP_MIN 1000
#define MONITOR_SET_TEMP_MAX 6000

/* -------------------------------------------------------------------------- */
/*                             Constants Interface                            */
/* -------------------------------------------------------------------------- */

// Pin definitions for various components
#define BUZZER_PIN 12
#define RED_LED_PIN 8
#define BLINK_PERIOD 1000 // Adjust to change LED blink speed

/* ---------------------- RGB LED Ring with FastLED Library ---------------------- */

// Number of LEDs connected in series
#define NUM_LEDS 45
#define LED_PIN 9
#define LED_BRIGHTNESS 35
#define DIM_WHITE 0x303030
#define BLINK_FAST_INTERVAL 250
#define BLINK_NORMAL_INTERVAL 1000
#define BLINK_SLOW_INTERVAL 1000

/* -------------------------- Buzzer Constants ------------------------------ */
#define BUZZER_FAST_INTERVAL 250
#define BUZZER_NORMAL_INTERVAL 500
#define BUZZER_SLOW_INTERVAL 1000

// Default LCD Address and dimensions
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

/* ------------------------------ Motor Control ----------------------------- */

// Motor Driver Pins for agitation and vibration
#define AGITATE_MOT_1 5
#define AGITATE_MOT_2 6
#define VIBRATE_MOT_1 11
#define VIBRATE_MOT_2 10

// PWM values for motor speed control
#define VIBRATE_PWM // Define your vibrate motor speed (0 - 255)
#define AGITATE_PWM // Define your agitate motor speed (0 - 255)

/* --------------------------------- Encoder -------------------------------- */

// Encoder pin definitions
#define ENC_CLK A1
#define ENC_DT 3
#define ENC_SW 4
#define LONG_PRESS_DUR 500 // Encoder button long press duration

/* --------------------------------- Sensors -------------------------------- */

// OneWire bus pin for temperature sensor
#define ONE_WIRE_BUS_PIN 7

/* -------------------------------------------------------------------------- */
/*                         Battery Charge Measurement                         */
/* -------------------------------------------------------------------------- */

// Kill switch pin for power interruption
#define KILL_BUS 2

#define BATT_SENSE_PIN A0
#define VREF 1071             // Reference voltage on pin 21 of ATMEGA. Change to match your setup.
#define CONVERSION_COEFF 2325 // Coefficient to convert analogRead value to voltage.
#define BATT_CHECK_PERIOD 250 // Adjust battery check frequency (in minutes)

// Battery charge thresholds
#define FULL_CHARGE_FLOOR 385            // 3.85 volts
#define MID_CHARGE_FLOOR 358             // 3.58 volts
#define LOW_CHARGE_FLOOR 350             // 3.5 volts
#define VERY_LOW_CHARGE_FLOOR 250        // 2.5 volts
#define BATTERY_DISCONNECTED_CEILING 125 // 1.25 volts

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                              Global Types                                  */
/* -------------------------------------------------------------------------- */

// States for the operation of the film development machine
enum OperationStateType
{
    IDLE,
    DEVELOPING,
    INDEVELOPMENU,
    MONITORING,
};

// Indicator states for LED and buzzer
enum IndicatorStateType
{
    BUSY,
    BUSYAuto,
    AVAILABLE
};

// Indicator parameters for control
enum IndicatorParamType
{
    START,
    STOP,
    TOGGLE
};

// Battery charge levels
enum ChargeLevelType
{
    FullCharge,
    MidCharge,
    LowCharge,
    VeryLowCharge,
    BatteryDisconnected
};

// Encoder input types
enum EncoderInputType
{
    EncoderNone,
    EncoderLeft,
    EncoderRight,
    EncoderEnter,
    EncoderExit
};

// Struct to hold various states and variables
struct StateType
{
    OperationStateType currentState;
    IndicatorStateType ledState;
    IndicatorStateType buzzerState;
    ChargeLevelType batteryLevel;
    unsigned long currentMillis;
    int setTemp;
    bool checkEncoder;
};

/* -------------------------------------------------------------------------- */
/*                              Global variables                              */
/* -------------------------------------------------------------------------- */

extern StateType State;              // Global state instance
extern EncoderInputType command;     // Encoder command
extern char tmpStr[STR_BUFF_LEN];    // Temporary string buffer
extern bool redrawMenu;              // Flag to redraw menu
extern bool shutDownRequestReceived; // Flag for shutdown request

#endif
