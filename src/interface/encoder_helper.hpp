#ifndef ENCODER_HELPER_HPP
#define ENCODER_HELPER_HPP

// Libraries
#include <Encoder.h>

// Header Files
#include "../../globals.h"
#include "../utilities/general_utilities.hpp"

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