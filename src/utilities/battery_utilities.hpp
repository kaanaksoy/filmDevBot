#ifndef BATTERY_UTILITIES_HPP
#define BATTERY_UTILITIES_HPP
#include <Arduino.h>
#include "../../debugUtils.h"
#include "../../globals.h"

namespace BatteryMonitor
{

    // Initialize battery charge measurement
    void initBatteryChargeMeasurement();

    // Update the charge level based on the battery voltage
    void updateChargeLevel(ChargeLevelType prevChargeLevel);

} // namespace BatteryWatcher
#endif