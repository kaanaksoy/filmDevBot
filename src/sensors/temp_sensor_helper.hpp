#ifndef TEMP_SENSOR_HELPER_HPP
#define TEMP_SENSOR_HELPER_HPP

// Define error values for temperature readings
#define SENSOR_ERR -99.9f
#define SENSOR_NOT_READY -8.8f

/* -------------------------------- Libraries ------------------------------- */
#include <OneWire.h>
#include <DallasTemperature.h>

/* --------------------------------- Headers -------------------------------- */
#include "../../globals.h"
#include "../../debugUtils.h"
#include "../interface/indicator_helpers.hpp"
#include "../utilities/state_manager.hpp"

namespace TempSensors
{
    extern OneWire oneWireBus;            // Setup onewire instance for comms.
    extern DallasTemperature tempSensors; // DallasTemperature instance
    extern DeviceAddress tankThermometer; // Address for tank sensor. change to fit yours.

    extern unsigned long requestTime; // Timestamp for temperature request

    // Initialize the temperature sensor
    void initializeTempSensor();

    // Request temperature from the tank sensor
    void requestTankTemp();

    // Get the tank temperature reading
    float getTankTemp();

} // namespace TempSensor

#endif