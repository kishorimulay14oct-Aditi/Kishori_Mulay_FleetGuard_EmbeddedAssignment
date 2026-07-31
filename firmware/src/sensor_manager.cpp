#include "sensor_manager.h"
#include "config.h"

/*
 * FleetGuard Sensor Manager
 *
 * This module handles:
 * - Sensor initialization
 * - Temperature/humidity acquisition
 * - Sensor data validation
 * - Sensor failure detection
 *
 * NOTE:
 * The actual sensor library and sensor model will be selected
 * during hardware integration.
 *
 * The current implementation provides the firmware interface
 * and validation structure without assuming a specific sensor.
 */

// Store the latest sensor values
static float lastTemperature = 0.0f;
static float lastHumidity = 0.0f;
static bool sensorInitialized = false;


bool sensorManagerBegin()
{
    /*
     * Sensor hardware initialization will be implemented here
     * after the final temperature/humidity sensor is selected.
     *
     * Examples:
     * - Initialize I2C sensor
     * - Initialize digital sensor
     * - Verify sensor communication
     */

    sensorInitialized = true;

    return sensorInitialized;
}


bool sensorManagerRead(SensorData& data)
{
    /*
     * If the sensor has not been initialized,
     * the reading is considered invalid.
     */
    if (!sensorInitialized)
    {
        data.valid = false;
        return false;
    }

    /*
     * TODO:
     * Replace these demonstration values with actual
     * sensor-library readings.
     *
     * Example:
     *
     * data.temperature = sensor.readTemperature();
     * data.humidity = sensor.readHumidity();
     */

    data.temperature = lastTemperature;
    data.humidity = lastHumidity;

    /*
     * Validate the acquired values.
     */
    data.valid = validateSensorData(data);

    return data.valid;
}


bool validateSensorData(const SensorData& data)
{
    /*
     * Check temperature limits.
     */
    if (data.temperature < MIN_VALID_TEMPERATURE ||
        data.temperature > MAX_VALID_TEMPERATURE)
    {
        return false;
    }

    /*
     * Check humidity limits.
     */
    if (data.humidity < MIN_VALID_HUMIDITY ||
        data.humidity > MAX_VALID_HUMIDITY)
    {
        return false;
    }

    return true;
}
