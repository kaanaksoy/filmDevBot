#ifndef GLOBALS_H
#define GLOBALS_H
#include <Arduino.h>
/* -------------------------------------------------------------------------- */
/*                             Global constants                             */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/* ---- All of the global constants, variables & pin definitions are here --- */
/* -------------------------------------------------------------------------- */

#define STR_BUFF_LEN 16

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

#define BUZZER_PIN 12
#define RED_LED_PIN 8
// Adjust to change how fast the LED blinks.
#define BLINK_PERIOD 1000

/* ---------------------- RGB LED Ring with FastLED Library ---------------------- */

// Number of LEDs connected in series
#define NUM_LEDS 45
#define LED_PIN 9
// (0-255) Sets max brightness
#define LED_BRIGHTNESS 35
// Custom colors
#define DIM_WHITE 0x303030

// Blink Speed Options
#define BLINK_FAST_INTERVAL 250
#define BLINK_NORMAL_INTERVAL 1000
#define BLINK_SLOW_INTERVAL 1000

/* -------------------------- Buzzer Constants ------------------------------ */
#define BUZZER_FAST_INTERVAL 250
#define BUZZER_NORMAL_INTERVAL 500
#define BUZZER_SLOW_INTERVAL 1000

// Default LCD Address that matches my unit. Change for your module.
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

// Locations to store the custom icons on the HITACHI driver for 16X02 LCD displays.
//  for more information please check createChar() documentation
#define LEFT_ARR_ICON_ADDR (char)60  // character built into LCD driver.
#define RIGHT_ARR_ICON_ADDR (char)62 // char built into LCD driver.
#define ENTER_ICON_ADDR 1
#define EXIT_ICON_ADDR 2
#define TANK_TEMP_ICON_ADDR 3
#define BATT_CHAR_ADDR 6

/* ------------------------------ Motor Control ----------------------------- */
/* Motor Driver Pins, based on the mini L298 motor driver, change to suit your
    needs.
*/

#define AGITATE_MOT_1 5
#define AGITATE_MOT_2 6
#define VIBRATE_MOT_1 11
#define VIBRATE_MOT_2 10

// The PWM value for the vibrate motor, use it to set your vibrate motor speed (0 - 255)
#define VIBRATE_PWM
// The PWM value for the agitate motor, use it to set your agitate motor speed (0 - 255)
#define AGITATE_PWM

/* --------------------------------- Encoder -------------------------------- */

// Encoder In A
#define ENC_CLK A1
// Encoder In B
#define ENC_DT 3
// Encoder Button
#define ENC_SW 4
// Encoder button long press duration
#define LONG_PRESS_DUR 500

/* --------------------------------- Sensors -------------------------------- */

// Data wire is plugged into port 6 on the Arduino
#define ONE_WIRE_BUS_PIN 7

/* -------------------------------------------------------------------------- */
/*                         Battery Charge Measurement                         */
/* -------------------------------------------------------------------------- */

#define POWEROFF_PIN 2

#define BATT_SENSE_PIN A0
#define VREF 1071             // Reference voltage on pin 21 of ATMEGA. Please change this to match yours.
#define CONVERSION_COEFF 2325 // Coefficient required to convert the 10-bit analogRead value to a voltage value.
// Adjust this constant to change how often battery is checked (in minutes).
#define BATT_CHECK_PERIOD 250 // 120000
/* The following thresholds are based on the dischage graph for the specific
    batteries used They may be incorrect for other batteries.

    Please refer to your battery's datasheet to modify the values

*/
#define FULL_CHARGE_FLOOR 385            // 3.85 volts
#define MID_CHARGE_FLOOR 358             // 3.58 volts
#define LOW_CHARGE_FLOOR 350             // 3.5 volts
#define VERY_LOW_CHARGE_FLOOR 250        // 2.5 volts
#define BATTERY_DISCONNECTED_CEILING 125 // 1.25 volts. No BMS should allow a lithium battery to get this low.

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                              Global Types                                  */
/* -------------------------------------------------------------------------- */
enum OperationStateType
{
    IDLE,
    DEVELOPING,
    MONITORING
};

// Indicators such as the LED and the buzzer use this to indicate status
enum IndicatorStateType
{
    BUSY,
    AVAILABLE
};
enum ChargeLevelType
{
    FullCharge,
    MidCharge,
    LowCharge,
    VeryLowCharge,
    BatteryDisconnected
};
// Define Encoder Navigation types
enum EncoderInputType
{
    EncoderNone,
    EncoderLeft,
    EncoderRight,
    EncoderEnter,
    EncoderExit
};
struct StateType
{
    // State variable
    OperationStateType currentState;
    // Make sure that no function can use the led if another function is doing so.
    IndicatorStateType ledState;
    // Follows LED
    IndicatorStateType buzzerState;
    // Record battery status
    ChargeLevelType batteryLevel;
    // Track global time. updated every loop of the main loop.
    unsigned long currentMillis;
    // Flag for encoder interrupt routine (true if moved)
    bool checkEncoder;
};
/* -------------------------------------------------------------------------- */
/*                              Global variables                              */
/* -------------------------------------------------------------------------- */

extern StateType State;

// String to be used to print to the LCD.
extern char tmpStr[STR_BUFF_LEN];

#endif