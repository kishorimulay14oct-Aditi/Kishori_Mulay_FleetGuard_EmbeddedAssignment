#include "sensor_manager.h"
#include "config.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>

/*
 * FleetGuard Sensor Manager
 *
 * Sensor:
 * SHT31
 *
 * Interface:
 * I2C
 *
 * Responsibilities:
 * - Initialize SHT31
 * - Read temperature
 * - Read relative humidity
 * - Validate sensor data
 * - Detect sensor communication failure
 */

static Adafruit_SHT31 sht31 = Adafruit_SHT31();

static bool sensorInitialized = false;


// ---------------------------------------------------------
// Sensor Initialization
// ---------------------------------------------------------

bool sensorManagerBegin()
{
    // Initialize I2C bus
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    // Initialize SHT31
    if (!sht31.begin(SHT31_I2C_ADDRESS))
    {
        Serial.println("ERROR: SHT31 sensor not detected.");

        sensorInitialized = false;

        return false;
    }

    sensorInitialized = true;

    Serial.println("SHT31 sensor initialized.");

    return true;
}


// ---------------------------------------------------------
// Sensor Reading
// ---------------------------------------------------------

bool sensorManagerRead(SensorData& data)
{
    // Default to invalid
    data.valid = false;

    if (!sensorInitialized)
    {
        Serial.println("ERROR: Sensor is not initialized.");

        return false;
    }

    // Read temperature
    float temperature = sht31.readTemperature();

    // Read relative humidity
    float humidity = sht31.readHumidity();

    /*
     * SHT31 returns NAN if the reading fails.
     */
    if (isnan(temperature) || isnan(humidity))
    {
        Serial.println("ERROR: Invalid SHT31 reading.");

        return false;
    }

    // Store readings
    data.temperature = temperature;
    data.humidity = humidity;

    // Validate readings
    data.valid = validateSensorData(data);

    if (!data.valid)
    {
        Serial.println("ERROR: Sensor data outside valid range.");

        return false;
    }

    return true;
}


// ---------------------------------------------------------
// Sensor Data Validation
// ---------------------------------------------------------

bool validateSensorData(const SensorData& data)
{
    /*
     * Temperature engineering validation range.
     */
    if (data.temperature < MIN_VALID_TEMPERATURE ||
        data.temperature > MAX_VALID_TEMPERATURE)
    {
        return false;
    }

    /*
     * Relative humidity must be between 0% and 100%.
     */
    if (data.humidity < MIN_VALID_HUMIDITY ||
        data.humidity > MAX_VALID_HUMIDITY)
    {
        return false;
    }

    return true;
}
