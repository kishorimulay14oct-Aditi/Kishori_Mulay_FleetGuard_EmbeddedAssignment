#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

/*
 * FleetGuard Sensor Manager
 *
 * Responsible for:
 * - Sensor initialization
 * - Temperature acquisition
 * - Humidity acquisition
 * - Sensor data validation
 * - Sensor failure detection
 */

struct SensorData
{
    float temperature;
    float humidity;
    bool valid;
};

/**
 * Initialize the environmental sensor.
 *
 * @return true if initialization is successful.
 */
bool sensorManagerBegin();

/**
 * Read temperature and humidity.
 *
 * @param data Reference to SensorData structure.
 * @return true if a valid reading is available.
 */
bool sensorManagerRead(SensorData& data);

/**
 * Validate sensor readings against configured limits.
 *
 * @param data Sensor reading.
 * @return true if the reading is valid.
 */
bool validateSensorData(const SensorData& data);

#endif
