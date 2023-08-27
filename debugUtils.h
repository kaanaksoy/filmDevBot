#ifdef DEBUG
// If DEBUG is defined, the following macros will be enabled for debugging.

// DEBUG_PRINT macro: Print the current timestamp and variable's value to Serial
#define DEBUG_PRINT(x)    \
  Serial.print(millis()); \
  Serial.print(": ");     \
  DUMP(x);                \
  // This macro prints the timestamp, a separator, and the value of x to the Serial port.

// DEBUG_TRACE macro: Print the current timestamp and a trace message to Serial
#define DEBUG_TRACE()     \
  Serial.print(millis()); \
  Serial.print(": ");     \
  TRACE();                \
  // This macro prints the timestamp and a trace message to the Serial port.

#else
// If DEBUG is not defined, the following macros will be disabled.

#define DEBUG_PRINT(x)

#define DEBUG_TRACE()

#endif
