#ifndef BATTERY_UTILITIES_HPP
#define BATTERY_UTILITIES_HPP
#include <Arduino.h>
#include "../../debugUtils.h"

// Battery charge measurement constants
#define BATTERY_SENSE_PIN A0
#define BATT_CHECK_INTERVAL 2 // Minutes
/* The following thresholds are based on experimentation with the specific
    ATMEGA Chip used in development. They may be inaccurate in other instances.

    In my case the values are best defined by the function:
        voltageRead = 212.365(batteryVoltage) + 140.332
*/
#define FULL_CHARGE_THRESHOLD 950 //
#define MID_CHARGE_THRESHOLD 870
#define LOW_CHARGE_THRESHOLD 780

namespace BatteryWatcher
{
    extern unsigned long lastBatteryCheckTime;

    enum batteryLevelType
    {
        FullCharge,
        MidCharge,
        LowCharge,
        VeryLowCharge
    };
    void initBatteryChargeMeasurement();

    // Returns in which range the battery charge level is.
    batteryLevelType autoMeasureChargeLevel();
} // namespace BatteryWatcher
#endif