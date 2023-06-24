#ifdef DEBUG
#define DEBUG_PRINT(x)    \
  Serial.print(millis()); \
  Serial.print(": ");     \
  DUMP(x);
#define DEBUG_TRACE()     \
  Serial.print(millis()); \
  Serial.print(": ");     \
  TRACE();
#else
#define DEBUG_PRINT(x)
#define DEBUG_TRACE()
#endif