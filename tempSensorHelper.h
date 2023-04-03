#pragma once

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "globals.h"
#include "debugUtils.h"

namespace TempSensors
{
    #define ONE_WIRE_BUS 6 // Data wire is plugged into port 6 on the Arduino
    #define TANK_SENSOR_INDEX 0
    OneWire oneWire(ONE_WIRE_BUS); // Setup onewire instance for comms.

    DallasTemperature tempSensors(&oneWire);

    void initializeTempSensor(){
        tempSensors.begin();
    }

    float getTankTemp(){
        DEBUG_PRINT("Requesting Temp");
        tempSensors.requestTemperatures();
        DEBUG_PRINT("Temp Received")
        float tempC = tempSensors.getTempCByIndex(TANK_SENSOR_INDEX);

        if (tempC != DEVICE_DISCONNECTED_C)
        {
            return tempC;
        } else
        {
            DEBUG_PRINT("Error: Could not read temperature data");
            return (-1.0);
        }
    }

} // namespace TempSensor
