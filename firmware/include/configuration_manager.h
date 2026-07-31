#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

/*
 * FleetGuard Configuration Manager
 *
 * Responsible for:
 * - Managing sampling interval
 * - Managing temperature thresholds
 * - Managing humidity thresholds
 * - Managing hysteresis values
 * - Loading configuration at startup
 * - Saving configuration for future persistent storage
 */

struct FleetGuardConfig
{
    unsigned long samplingIntervalMs;

    float temperatureWarningEnter;
    float temperatureWarningExit;

    float temperatureCriticalEnter;
    float temperatureCriticalExit;

    float humidityWarningEnter;
    float humidityWarningExit;

    float humidityCriticalEnter;
    float humidityCriticalExit;
};

/**
 * Initialize the configuration manager.
 */
bool configurationManagerBegin();

/**
 * Load configuration.
 *
 * Initially loads the engineering demonstration values
 * defined in config.h.
 */
bool loadConfiguration(FleetGuardConfig& config);

/**
 * Save configuration.
 *
 * Future implementation will store the configuration
 * in ESP32 non-volatile storage.
 */
bool saveConfiguration(const FleetGuardConfig& config);

#endif
