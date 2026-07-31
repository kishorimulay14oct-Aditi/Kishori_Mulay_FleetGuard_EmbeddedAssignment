#include "configuration_manager.h"
#include "config.h"
#include <Arduino.h>

static bool configurationInitialized = false;

bool configurationManagerBegin()
{
    configurationInitialized = true;

    Serial.println("Configuration Manager initialized.");

    return configurationInitialized;
}


bool loadConfiguration(FleetGuardConfig& config)
{
    if (!configurationInitialized)
    {
        return false;
    }

    /*
     * Load the engineering demonstration values.
     *
     * Future implementation:
     * Read saved configuration from ESP32 NVS.
     */

    config.samplingIntervalMs =
        DEFAULT_SAMPLING_INTERVAL_MS;

    config.temperatureWarningEnter =
        TEMPERATURE_WARNING_ENTER;

    config.temperatureWarningExit =
        TEMPERATURE_WARNING_EXIT;

    config.temperatureCriticalEnter =
        TEMPERATURE_CRITICAL_ENTER;

    config.temperatureCriticalExit =
        TEMPERATURE_CRITICAL_EXIT;

    config.humidityWarningEnter =
        HUMIDITY_WARNING_ENTER;

    config.humidityWarningExit =
        HUMIDITY_WARNING_EXIT;

    config.humidityCriticalEnter =
        HUMIDITY_CRITICAL_ENTER;

    config.humidityCriticalExit =
        HUMIDITY_CRITICAL_EXIT;

    Serial.println("Configuration loaded.");

    return true;
}


bool saveConfiguration(const FleetGuardConfig& config)
{
    if (!configurationInitialized)
    {
        return false;
    }

    /*
     * Future implementation:
     *
     * Store configuration in ESP32 NVS.
     *
     * The configuration will then survive a power restart.
     */

    Serial.println("Configuration save requested.");

    return true;
}
