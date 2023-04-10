#ifndef ENCODER_HELPER_HPP
#define ENCODER_HELPER_HPP

// Libraries
#include <Encoder.h>
// Header Files
#include "../../globals.h"
#include "../utilities/general_utilities.hpp"

// Encoder Pins
/*
(Using Encoder Breakout Board with pullip resistors.
If you dont have pullup resistors, you can enable the built in ones.
*/
#define ENC_CLK 2          // Encoder In B
#define ENC_DT 3           // Encoder In A
#define ENC_SW 4           // Encoder Button
#define LONG_PRESS_DUR 500 // Button long press duration

namespace SystemEncoder
{
    extern Encoder gEncoder; // Initialize Encoder

    // Define Encoder Navigation types
    enum EncoderInputType
    {
        EncoderNone,
        EncoderLeft,
        EncoderRight,
        EncoderEnter,
        EncoderExit
    };

    // --- getCommand | Menu Navigation Function ---
    EncoderInputType getCommand(EncoderInputType prevCommand);

    EncoderInputType encoderAwaitConfirm();
} // namespace Encoder

#endif