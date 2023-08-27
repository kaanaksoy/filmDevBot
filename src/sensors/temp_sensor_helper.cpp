
#include "temp_sensor_helper.hpp"

namespace TempSensors
{
    unsigned long requestTime;            // Timestamp for temp request.
    OneWire oneWireBus(ONE_WIRE_BUS_PIN); // Setup OneWire instance for communication.
    DallasTemperature tempSensors(&oneWireBus);
    DeviceAddress tankThermometer = {0x28, 0xC6, 0x74, 0x57, 0x04, 0xE1, 0x3D, 0x73};

    void initializeTempSensor()
    {
        // Initialize the temperature sensor and OneWire communication.
        DEBUG_PRINT("Locating devices");
        tempSensors.begin();
        DEBUG_PRINT("Devices found");
        DEBUG_PRINT(tempSensors.getDeviceCount());
        DEBUG_PRINT("Devices");
        tempSensors.setResolution(10);           // Set temperature resolution to 10 bits.
        tempSensors.setWaitForConversion(false); // Don't wait for conversion.
    }

    void requestTankTemp()
    {
        // Request temperature data from the tankThermometer sensor.
        if (State.currentMillis - requestTime >= 400)
        {
            tempSensors.requestTemperaturesByAddress(tankThermometer);
            requestTime = State.currentMillis;
        }
    }

    float getTankTemp()
    {
        // Request temperature data from the tankThermometer sensor.
        if (State.currentMillis - requestTime >= 200)
        {
            float tempC = tempSensors.getTempC(tankThermometer);
            if (tempC != DEVICE_DISCONNECTED_C) // Check for valid temperature reading
            {
                return tempC; // Return the temperature in Celsius.
            }
            else
            {
                DEBUG_PRINT("Error: Could not read temperature data");
                return SENSOR_ERR; // Return an error value if reading fails.
            }
        }
        else
        {
            return SENSOR_NOT_READY; // Return indication that the sensor is not ready yet.
        }
    }

} // namespace TempSensor