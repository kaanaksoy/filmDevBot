#pragma once

// Battery charge measurement constants
#define BATTERY_SENSE_PIN A0
#define BATT_CHECK_INTERVAL 2 //Minutes
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
    unsigned long lastBatteryCheckTime = 0;

    // Define the levels charge to be displayed
    enum batteryLevel {
        FullCharge,
        MidCharge,
        LowCharge,
        VeryLowCharge
    };
    void initBatteryChargeMeasurement(){
       	analogReference(EXTERNAL);
        DEBUG_PRINT("Reading from AREF EXTERNAL");
        lastBatteryCheckTime = millis();
    }

    // Returns in which range the battery charge level is.
    batteryLevel autoMeasureChargeLevel() {
        
        int voltageRead = analogRead(BATTERY_SENSE_PIN); //Read battery value.

        if (voltageRead >= FULL_CHARGE_THRESHOLD)
        {
            return FullCharge;
        } else if (voltageRead >= MID_CHARGE_THRESHOLD)
        {
            return MidCharge;
        } else if (voltageRead >= LOW_CHARGE_THRESHOLD)
        {
            return LowCharge;
        }else
        {
            return VeryLowCharge;
        }
        
    }
} // namespace BatteryWatcher
