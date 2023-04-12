#ifndef BATTERY_UTILITIES_HPP
#define BATTERY_UTILITIES_HPP
#include <Arduino.h>
#include "../../debugUtils.h"
#include "../../globals.h"
#include "state_manager.hpp"

namespace BatteryMonitor
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
    batteryLevelType measureChargeLevel();

    void powerOff();
} // namespace BatteryWatcher
#endif