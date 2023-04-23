#ifndef BATTERY_UTILITIES_HPP
#define BATTERY_UTILITIES_HPP
#include <Arduino.h>
#include "../../debugUtils.h"
#include "../../globals.h"

namespace BatteryMonitor
{
    void initBatteryChargeMeasurement();

    // Returns in which range the battery charge level is.
    void updateChargeLevel(ChargeLevelType prevChargeLevel);

    void powerOff();
} // namespace BatteryWatcher
#endif