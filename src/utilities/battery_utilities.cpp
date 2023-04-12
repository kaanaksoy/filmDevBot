
#include "battery_utilities.hpp"

namespace BatteryMonitor
{
    unsigned long lastBatteryCheckTime = 0;

    void initBatteryChargeMeasurement()
    {
        analogReference(EXTERNAL);
        DEBUG_PRINT("Reading from AREF EXTERNAL");
        lastBatteryCheckTime = currentMillis;
    }

    // Returns in which range the battery charge level is.
    batteryLevelType measureChargeLevel()
    {

        int voltageRead = analogRead(BATTERY_SENSE_PIN); // Read battery value.
        lastBatteryCheckTime = currentMillis;

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

    void powerOff()
    {
        return;
    }
} // namespace BatteryWatcher
