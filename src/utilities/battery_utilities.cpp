
#include "battery_utilities.hpp"

namespace BatteryMonitor
{
    // Initialize battery charge measurement
    void initBatteryChargeMeasurement()
    {
        analogReference(INTERNAL); // Set the analog reference to internal (1.1V) reference
        DEBUG_PRINT("Reading from AREF INTERNAL");
        updateChargeLevel(State.batteryLevel); // Update the initial charge level
    }

    // Update the charge level based on the battery voltage
    void updateChargeLevel(ChargeLevelType prevChargeLevel)
    {
        static unsigned long lastBatteryCheckTime; // Store the last time battery level was checked

        // Return if it's not time to update the battery level yet
        if (State.currentMillis < lastBatteryCheckTime + BATT_CHECK_PERIOD)
        {
            return;
        }

        // Read the voltage from the battery sense pin and convert to millivolts
        unsigned long voltageRead = (unsigned long)analogRead(BATT_SENSE_PIN) * VREF / CONVERSION_COEFF;
        lastBatteryCheckTime = State.currentMillis;

        // Determine the charge level based on voltage thresholds
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
} // namespace BatteryWatcher
