# Film Processing Machine

Welcome to the README file for my film processing machine's codebase. This document provides an overview of the code structure, its components, and how to navigate and use the various functionalities.

## Table of Contents

-   [Overview](#overview)
-   [Dependencies](#dependencies)
-   [Code Structure](#code-structure)
-   [Usage](#usage)
-   [Debugging](#debugging)
-   [Contributing](#contributing)
-   [License](#license)

## Overview

This project implements a system for monitoring and controlling various aspects of the film development machine. It includes modules for handling encoder input, LED indicators, LCD display, temperature sensing, battery monitoring, and menu navigation.

## Dependencies

-   Arduino IDE
-   Arduino Library (included with Arduino IDE)
-   Arduino Trace Library
-   FastLED library
-   hd44780 library
-   OneWire library
-   DallasTemperature library
-   CMBMenu Library
-   LCD_I2C Library

## Code Structure

The codebase is organized into several modules, each responsible for a specific aspect of the system:

-   **Encoder Module** (`encoder_helper.hpp` and `encoder_helper.cpp`): Handles input from an encoder device for menu navigation.

-   **LED Indicator Module** (`led_helper.hpp` and `led_helper.cpp`): Manages LED indicators' behavior, such as blinking and progress bar display.

-   **LCD Display Module** (`lcd_helper.hpp` and `lcd_helper.cpp`): Manages the communication with an LCD display and handles custom character creation for icons.

-   **Menu UI Module** (`menu_helper.hpp` and `menu_helper.cpp`): Creates and manages the user interface menu using a CMBMenu library.

-   **Icons Module** (`icons.hpp`): Defines custom icons for display on the LCD.

-   **Temperature Sensor Module** (`temp_sensor_helper.hpp` and `temp_sensor_helper.cpp`): Interfaces with temperature sensors using the OneWire and DallasTemperature libraries.

-   **Battery Utilities Module** (`battery_utilities.hpp` and `battery_utilities.cpp`): Manages battery charge level measurement and updates.

-   **State Manager Module** (`state_manager.hpp` and `state_manager.cpp`): Controls the operation state of the system.

Each module contains its respective header and implementation files. The `globals.h` header file centralizes global variables, enums, and constants used across the codebase.

The main file is filmDevBot.ino.

## Usage

1. Install the required libraries using the Arduino Library Manager.
2. Include the necessary header files in your main sketch.
3. Initialize modules as needed using their respective initialization functions.
4. Implement your application logic, utilizing the provided functions to interact with various hardware components.

## Debugging

The codebase includes debugging macros that can be enabled or disabled by defining or undefining the `DEBUG` preprocessor symbol. When `DEBUG` is defined, the macros `DEBUG_PRINT(x)` and `DEBUG_TRACE()` can be used to print debugging information to the Serial port.

## Contributing

Contributions to this project are welcome. If you find any issues or have improvements to suggest, feel free to submit a pull request or open an issue in the repository.

## License

This project is licensed under the [MIT License](LICENSE).

---

Feel free to customize and enhance this README according to your project's specific details and needs.
