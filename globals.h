#pragma once

#define RED_LED 7

// Motor Driver Pins
// (Using Mini L298 Motor Driver Board)
#define AGITATE_MOT_1 8  // Agitate Motor 1
#define AGITATE_MOT_2 9  // Agitate Motor 2
#define VIBRATE_MOT_1 10 // Vibrate Motor 1
#define VIBRATE_MOT_2 11 // Vibrate Motor 2

#define LEFT_ARR_ICON_ADDR 1
#define ENTER_ICON_ADDR 2
#define RIGHT_ARR_ICON_ADDR 3
#define EXIT_ICON_ADDR 4
#define TANK_TEMP_ICON_ADDR 5
#define BATT_CHAR_ADDR 6

#define LCD_ADDR 0x27 // Default LCD Address. Change for your module.

// C-41 Process values
#define MAX_PULL -1 // Maximum stops that we can pull the film
#define MAX_PUSH 3  // Maximum stops that we can push the film

// Durations for the specific processes. Based on the values from the CineStill Cs-41 Kit.
#define PULL_ONE_DUR 165
#define STD_DEV_DUR 210
#define PUSH_ONE_DUR 273
#define PUSH_TWO_DUR 368
#define PUSH_THR_DUR 525
#define FIXING_TIME 480