#include "configuration_manager.h"
#include "config.h"

#include <Arduino.h>
#include <Preferences.h>

static bool configurationInitialized = false;

static Preferences preferences;


// ---------------------------------------------------------
// Configuration Manager Initialization
// ---------------------------------------------------------

bool configurationManagerBegin()
{
    /*
     * Open the FleetGuard NVS namespace.
     *
     * "fleetguard" is the storage namespace used by
     * this application.
     */
    if (!preferences.begin("fleetguard", false))
    {
        Serial.println("ERROR: NVS initialization failed.");

        configurationInitialized = false;

        return false;
    }

    configurationInitialized = true;

    Serial.println("Configuration Manager initialized.");

    return true;
}


// ---------------------------------------------------------
// Load Configuration
// ---------------------------------------------------------

bool loadConfiguration(FleetGuardConfig& config)
{
    if (!configurationInitialized)
    {
        return false;
    }

    /*
     * Check whether a saved configuration exists.
     */
    bool configurationExists =
        preferences.getBool("configured", false);

    if (!configurationExists)
    {
        /*
         * No saved configuration.
         *
         * Load engineering demonstration values.
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

        Serial.println(
            "No saved configuration found."
        );

        Serial.println(
            "Using default engineering values."
        );

        return true;
    }

    /*
     * Load saved configuration from NVS.
     */
    config.samplingIntervalMs =
        preferences.getULong(
            "sample_ms",
            DEFAULT_SAMPLING_INTERVAL_MS
        );

    config.temperatureWarningEnter =
        preferences.getFloat(
            "temp_w_enter",
            TEMPERATURE_WARNING_ENTER
        );

    config.temperatureWarningExit =
        preferences.getFloat(
            "temp_w_exit",
            TEMPERATURE_WARNING_EXIT
        );

    config.temperatureCriticalEnter =
        preferences.getFloat(
            "temp_c_enter",
            TEMPERATURE_CRITICAL_ENTER
        );

    config.temperatureCriticalExit =
        preferences.getFloat(
            "temp_c_exit",
            TEMPERATURE_CRITICAL_EXIT
        );

    config.humidityWarningEnter =
        preferences.getFloat(
            "hum_w_enter",
            HUMIDITY_WARNING_ENTER
        );

    config.humidityWarningExit =
        preferences.getFloat(
            "hum_w_exit",
            HUMIDITY_WARNING_EXIT
        );

    config.humidityCriticalEnter =
        preferences.getFloat(
            "hum_c_enter",
            HUMIDITY_CRITICAL_ENTER
        );

    config.humidityCriticalExit =
        preferences.getFloat(
            "hum_c_exit",
            HUMIDITY_CRITICAL_EXIT
        );

    Serial.println(
        "Saved configuration loaded from NVS."
    );

    return true;
}


// ---------------------------------------------------------
// Save Configuration
// ---------------------------------------------------------

bool saveConfiguration(
    const FleetGuardConfig& config)
{
    if (!configurationInitialized)
    {
        return false;
    }

    /*
     * Store configuration in ESP32 NVS.
     */
    preferences.putULong(
        "sample_ms",
        config.samplingIntervalMs
    );

    preferences.putFloat(
        "temp_w_enter",
        config.temperatureWarningEnter
    );

    preferences.putFloat(
        "temp_w_exit",
        config.temperatureWarningExit
    );

    preferences.putFloat(
        "temp_c_enter",
        config.temperatureCriticalEnter
    );

    preferences.putFloat(
        "temp_c_exit",
        config.temperatureCriticalExit
    );

    preferences.putFloat(
        "hum_w_enter",
        config.humidityWarningEnter
    );

    preferences.putFloat(
        "hum_w_exit",
        config.humidityWarningExit
    );

    preferences.putFloat(
        "hum_c_enter",
        config.humidityCriticalEnter
    );

    preferences.putFloat(
        "hum_c_exit",
        config.humidityCriticalExit
    );

    /*
     * Mark configuration as valid.
     */
    preferences.putBool(
        "configured",
        true
    );

    Serial.println(
        "Configuration saved to NVS."
    );

    return true;
}
