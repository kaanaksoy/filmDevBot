
#include "temp_sensor_helper.hpp"

namespace TempSensors
{
    unsigned long requestTime;            // Timestamp for temp request.
    OneWire oneWireBus(ONE_WIRE_BUS_PIN); // Setup onewire instance for comms.
    DallasTemperature tempSensors(&oneWireBus);
    DeviceAddress tankThermometer = {0x28, 0x3B, 0x00, 0x97, 0x94, 0x07, 0x03, 0x56}; // Addr for tank sensor. change to fit yours.

    void initializeTempSensor()
    {
        // oneWireBus.begin(ONE_WIRE_BUS_PIN);
        // tempSensors.setOneWire(&oneWireBus);
        DEBUG_PRINT("Locating devices");
        tempSensors.begin();
        DEBUG_PRINT("Devices found");
        DEBUG_PRINT(tempSensors.getDeviceCount());
        DEBUG_PRINT("Devices");
        tempSensors.setResolution(10);
        tempSensors.setWaitForConversion(false);
    }

    void requestTankTemp()
    {
        if (millis() - requestTime >= 400)
        {
            tempSensors.requestTemperaturesByAddress(tankThermometer);
            requestTime = millis();
        }
    }

    float getTankTemp()
    {

        if (millis() - requestTime >= 200)
        {
            float tempC = tempSensors.getTempC(tankThermometer);
            if (tempC != DEVICE_DISCONNECTED_C)
            {
                return tempC;
            }
            else
            {
                DEBUG_PRINT("Error: Could not read temperature data");
                return SENSOR_ERR;
            }
        }
        else
        {
            return SENSOR_NOT_READY;
        }
    }

} // namespace TempSensor