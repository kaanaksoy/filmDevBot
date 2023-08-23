
#include "battery_utilities.hpp"

namespace BatteryMonitor
{
    void initBatteryChargeMeasurement()
    {
        analogReference(INTERNAL);
        DEBUG_PRINT("Reading from AREF INTERNAL");
        updateChargeLevel(State.batteryLevel);
    }

    // Returns in which range the battery charge level is.
    void updateChargeLevel(ChargeLevelType prevChargeLevel)
    {
        static unsigned long lastBatteryCheckTime;

        // Return if we don't need to update the battery level.
        if (State.currentMillis < lastBatteryCheckTime + BATT_CHECK_PERIOD)
        {
            return;
        }

        // Calculatesthe battery voltage.
        // Conversion coefficient is required to convert the voltage based on the voltage divider used.
        unsigned long voltageRead = (unsigned long)analogRead(BATT_SENSE_PIN) * VREF / CONVERSION_COEFF;
        lastBatteryCheckTime = State.currentMillis;

        if (voltageRead >= FULL_CHARGE_FLOOR)
        {
            State.batteryLevel = FullCharge;
            return;
        }
        else if (voltageRead >= MID_CHARGE_FLOOR)
        {
            State.batteryLevel = MidCharge;
            return;
        }
        else if (voltageRead >= LOW_CHARGE_FLOOR)
        {
            State.batteryLevel = LowCharge;
            return;
        }
        else if (voltageRead >= VERY_LOW_CHARGE_FLOOR)
        {
            State.batteryLevel = VeryLowCharge;
            return;
        }
        else if (voltageRead <= BATTERY_DISCONNECTED_CEILING)
        {
            State.batteryLevel = BatteryDisconnected;
            return;
        }
    }

    void handlePowerOffRequest() { return; }

} // namespace BatteryWatcher
