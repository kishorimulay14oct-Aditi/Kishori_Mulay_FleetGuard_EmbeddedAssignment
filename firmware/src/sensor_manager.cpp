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
    /*
     * Initialize the ESP32 I2C bus.
     *
     * SDA = GPIO 21
     * SCL = GPIO 22
     */
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    /*
     * Initialize SHT31 using the configured I2C address.
     */
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
    /*
     * Always start with an invalid data state.
     */
    data.valid = false;

    if (!sensorInitialized)
    {
        Serial.println("ERROR: Sensor is not initialized.");

        return false;
    }

    /*
     * Read temperature.
     */
    float temperature = sht31.readTemperature();

    /*
     * Read relative humidity.
     */
    float humidity = sht31.readHumidity();

    /*
     * The SHT31 library returns NAN when a measurement
     * cannot be obtained.
     */
    if (isnan(temperature) || isnan(humidity))
    {
        Serial.println("ERROR: Invalid SHT31 reading.");

        return false;
    }

    /*
     * Store the sensor measurements.
     */
    data.temperature = temperature;
    data.humidity = humidity;

    /*
     * Validate the measurements before making them
     * available to the condition classifier.
     */
    if (!validateSensorData(data))
    {
        Serial.println("ERROR: Sensor data outside valid range.");

        return false;
    }

    data.valid = true;

    return true;
}


// ---------------------------------------------------------
// Sensor Data Validation
// ---------------------------------------------------------

bool validateSensorData(const SensorData& data)
{
    /*
     * Validate temperature against the configured
     * engineering limits.
     */
    if (data.temperature < MIN_VALID_TEMPERATURE ||
        data.temperature > MAX_VALID_TEMPERATURE)
    {
        return false;
    }

    /*
     * Validate relative humidity.
     */
    if (data.humidity < MIN_VALID_HUMIDITY ||
        data.humidity > MAX_VALID_HUMIDITY)
    {
        return false;
    }

    return true;
}
