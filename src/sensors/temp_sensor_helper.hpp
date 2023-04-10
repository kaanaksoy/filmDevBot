#ifndef TEMP_SENSOR_HELPER_HPP
#define TEMP_SENSOR_HELPER_HPP

#include <OneWire.h>
#include <DallasTemperature.h>

#include "../../globals.h"
#include "../../debugUtils.h"
#include "../utilities/general_utilities.hpp"

#define ONE_WIRE_BUS_PIN 6     // Data wire is plugged into port 6 on the Arduino
#define SENSOR_ERR -99.9f      // Used in comm with temp sensors
#define SENSOR_NOT_READY -8.8f // Used for comm with temp sensors

namespace TempSensors
{
    extern OneWire oneWireBus; // Setup onewire instance for comms.
    extern DallasTemperature tempSensors;
    extern DeviceAddress tankThermometer; // Addr for tank sensor. change to fit yours.

    extern unsigned long requestTime; // Timestamp for temp request.

    void initializeTempSensor();

    void requestTankTemp();

    float getTankTemp();

} // namespace TempSensor

#endif