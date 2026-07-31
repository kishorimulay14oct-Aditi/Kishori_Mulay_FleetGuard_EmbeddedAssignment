#ifndef CONNECTIVITY_MANAGER_H
#define CONNECTIVITY_MANAGER_H

/*
 * FleetGuard Connectivity Manager
 *
 * Responsible for future network connectivity:
 * - Wi-Fi connection
 * - MQTT communication
 * - Telemetry publishing
 *
 * Local monitoring shall continue even when
 * network connectivity is unavailable.
 */

bool connectivityManagerBegin();

/**
 * Attempt to connect to the configured Wi-Fi network.
 *
 * @return true if connected.
 */
bool connectToWiFi();

/**
 * Maintain the network connection.
 */
void maintainConnectivity();

/**
 * Publish telemetry data through MQTT.
 *
 * @param temperature Current temperature.
 * @param humidity Current humidity.
 * @param state Current environmental condition.
 *
 * @return true if telemetry was published.
 */
bool publishTelemetry(
    float temperature,
    float humidity,
    const char* state
);

#endif
