/*
 * FleetGuard Intelligent Refrigerated Container Monitoring System
 *
 * Main Application
 *
 * Responsibilities:
 * - Initialize firmware modules
 * - Load configuration
 * - Read environmental sensors
 * - Validate sensor data
 * - Classify environmental condition
 * - Apply hysteresis
 * - Control local alerts
 * - Log important events
 * - Maintain periodic monitoring
 * - Maintain optional Wi-Fi/MQTT connectivity
 */

#include <Arduino.h>

#include "config.h"
#include "sensor_manager.h"
#include "condition_classifier.h"
#include "alert_manager.h"
#include "event_logger.h"
#include "configuration_manager.h"
#include "connectivity_manager.h"


// ---------------------------------------------------------
// Global State
// ---------------------------------------------------------

static ConditionState currentState = ConditionState::NORMAL;

static FleetGuardConfig systemConfig;

static bool previousSensorValid = true;

static unsigned long lastSampleTime = 0;


// ---------------------------------------------------------
// Setup
// ---------------------------------------------------------

void setup()
{
    // Initialize serial debugging first
    Serial.begin(115200);

    delay(500);

    Serial.println();
    Serial.println("=================================");
    Serial.println(" FleetGuard Monitoring System");
    Serial.println(" Starting system...");
    Serial.println("=================================");


    // -----------------------------------------------------
    // Configuration Manager
    // -----------------------------------------------------

    if (!configurationManagerBegin())
    {
        Serial.println("ERROR: Configuration Manager failed.");
    }

    if (!loadConfiguration(systemConfig))
    {
        Serial.println("ERROR: Configuration loading failed.");
    }


    // -----------------------------------------------------
    // Sensor Manager
    // -----------------------------------------------------

    if (!sensorManagerBegin())
    {
        Serial.println("ERROR: Sensor initialization failed.");
    }


    // -----------------------------------------------------
    // Alert Manager
    // -----------------------------------------------------

    if (!alertManagerBegin())
    {
        Serial.println("ERROR: Alert Manager initialization failed.");
    }


    // -----------------------------------------------------
    // Event Logger
    // -----------------------------------------------------

    if (!eventLoggerBegin())
    {
        Serial.println("ERROR: Event Logger initialization failed.");
    }


    // -----------------------------------------------------
    // Connectivity Manager
    // -----------------------------------------------------

    if (!connectivityManagerBegin())
    {
        Serial.println("WARNING: Connectivity Manager failed.");
    }

    // Network connectivity is optional.
    // Local monitoring must continue even if Wi-Fi is unavailable.
    connectToWiFi();


    // -----------------------------------------------------
    // Record System Startup
    // -----------------------------------------------------

    logEvent(EventType::SYSTEM_START);


    // -----------------------------------------------------
    // Initialize Sampling Timer
    // -----------------------------------------------------

    lastSampleTime = millis();

    Serial.println("System initialization complete.");
}


// ---------------------------------------------------------
// Main Monitoring Loop
// ---------------------------------------------------------

void loop()
{
    unsigned long currentTime = millis();


    // -----------------------------------------------------
    // Maintain Optional Connectivity
    // -----------------------------------------------------

    maintainConnectivity();


    // -----------------------------------------------------
    // Sampling Interval
    // -----------------------------------------------------

    if (currentTime - lastSampleTime <
        systemConfig.samplingIntervalMs)
    {
        return;
    }

    lastSampleTime = currentTime;


    // -----------------------------------------------------
    // Read Sensor
    // -----------------------------------------------------

    SensorData sensorData;

    bool sensorReadSuccessful =
        sensorManagerRead(sensorData);


    // -----------------------------------------------------
    // Sensor Failure Handling
    // -----------------------------------------------------

    if (!sensorReadSuccessful || !sensorData.valid)
    {
        Serial.println("ERROR: Invalid sensor data.");

        showSensorFault();

        // Log failure only when entering the fault state
        if (previousSensorValid)
        {
            logEvent(EventType::SENSOR_FAILURE);
        }

        previousSensorValid = false;

        return;
    }


    // -----------------------------------------------------
    // Sensor Recovery
    // -----------------------------------------------------

    if (!previousSensorValid)
    {
        logEvent(EventType::SENSOR_RECOVERY);

        previousSensorValid = true;
    }


    // -----------------------------------------------------
    // Condition Classification
    // -----------------------------------------------------

    ConditionState previousState = currentState;

    currentState = classifyCondition(
        sensorData.temperature,
        sensorData.humidity,
        previousState
    );


    // -----------------------------------------------------
    // Detect State Changes
    // -----------------------------------------------------

    if (currentState != previousState)
    {
        if (previousState == ConditionState::NORMAL &&
            currentState == ConditionState::WARNING)
        {
            logEvent(EventType::NORMAL_TO_WARNING);
        }
        else if (previousState == ConditionState::WARNING &&
                 currentState == ConditionState::CRITICAL)
        {
            logEvent(EventType::WARNING_TO_CRITICAL);
        }
        else if (previousState == ConditionState::CRITICAL &&
                 currentState == ConditionState::WARNING)
        {
            logEvent(EventType::CRITICAL_TO_WARNING);
        }
        else if (previousState == ConditionState::WARNING &&
                 currentState == ConditionState::NORMAL)
        {
            logEvent(EventType::WARNING_TO_NORMAL);
        }
    }


    // -----------------------------------------------------
    // Update Local Alerts
    // -----------------------------------------------------

    updateAlerts(
        currentState,
        sensorData.temperature,
        sensorData.humidity
    );


    // -----------------------------------------------------
    // Optional MQTT Telemetry
    // -----------------------------------------------------

    publishTelemetry(
        sensorData.temperature,
        sensorData.humidity,
        conditionStateToString(currentState)
    );


    // -----------------------------------------------------
    // Debug Information
    // -----------------------------------------------------

    Serial.print("Temperature: ");
    Serial.print(sensorData.temperature);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(sensorData.humidity);
    Serial.println(" %RH");

    Serial.print("Current State: ");
    Serial.println(
        conditionStateToString(currentState)
    );
}
