
#include "battery_utilities.hpp"

namespace BatteryWatcher
{
    unsigned long lastBatteryCheckTime = 0;

    void initBatteryChargeMeasurement()
    {
        analogReference(EXTERNAL);
        DEBUG_PRINT("Reading from AREF EXTERNAL");
        lastBatteryCheckTime = millis();
    }

    // Returns in which range the battery charge level is.
    batteryLevelType autoMeasureChargeLevel()
    {

        int voltageRead = analogRead(BATTERY_SENSE_PIN); // Read battery value.

        if (voltageRead >= FULL_CHARGE_THRESHOLD)
        {
            return FullCharge;
        }
        else if (voltageRead >= MID_CHARGE_THRESHOLD)
        {
            return MidCharge;
        }
        else if (voltageRead >= LOW_CHARGE_THRESHOLD)
        {
            return LowCharge;
        }
        else
        {
            return VeryLowCharge;
        }
    }
} // namespace BatteryWatcher
