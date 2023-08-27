#ifndef ENCODER_HELPER_HPP
// Define the header guard symbol to prevent multiple inclusion
#define ENCODER_HELPER_HPP

// Header Files
#include "../../globals.h"                    // Include necessary global headers
#include "../interface/indicator_helpers.hpp" // Include indicator helper functions

namespace SystemEncoder
{
    // Initialize Encoder
    void initEncoder(); // Declare a function to initialize the encoder

    // Encoder Interrupt Service Routine (ISR)
    void enc_ISR(); // Declare the ISR function for encoder input

    // Await User Confirmation Using Encoder
    EncoderInputType encoderAwaitConfirm();

    // --- getCommand | Menu Navigation Function ---
    EncoderInputType getCommand(EncoderInputType prevCommand);
    // Declare a function to get the user's command for menu navigation
    // prevCommand: The previously selected command
} // namespace Encoder

#endif