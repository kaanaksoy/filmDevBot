#ifndef ENCODER_HELPER_HPP
#define ENCODER_HELPER_HPP

// Header Files
#include "../../globals.h"
#include "../utilities/general_utilities.hpp"

namespace SystemEncoder
{
    // Initialize Encoder
    void initEncoder();

    void enc_ISR();

    EncoderInputType encoderAwaitConfirm();

    // --- getCommand | Menu Navigation Function ---
    EncoderInputType getCommand(EncoderInputType prevCommand);
} // namespace Encoder

#endif