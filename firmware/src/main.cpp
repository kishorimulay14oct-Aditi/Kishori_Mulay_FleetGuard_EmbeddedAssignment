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
 */

#include <Arduino.h>

#include "config.h"
#include "sensor_manager.h"
#include "condition_classifier.h"
#include "alert_manager.h"
#include "event_logger.h"
#include "configuration_manager.h"

// Current system state
static ConditionState currentState = ConditionState::NORMAL;

// Current configuration
static FleetGuardConfig systemConfig;

// Previous sensor validity
static bool previousSensorValid = true;

// Timing
static unsigned long lastSampleTime = 0;


// ---------------------------------------------------------
// Setup
// ---------------------------------------------------------

void setup()
{
    Serial.begin(115200);

    delay(500);

    Serial.println();
    Serial.println("=================================");
    Serial.println(" FleetGuard Monitoring System");
    Serial.println(" Starting system...");
    Serial.println("=================================");

    // Initialize configuration manager
    if (!configurationManagerBegin())
    {
        Serial.println("ERROR: Configuration Manager failed.");
    }

    // Load configuration
    if (!loadConfiguration(systemConfig))
    {
        Serial.println("ERROR: Configuration loading failed.");
    }

    // Initialize sensor manager
    if (!sensorManagerBegin())
    {
        Serial.println("ERROR: Sensor initialization failed.");
    }

    // Initialize alert manager
    if (!alertManagerBegin())
    {
        Serial.println("ERROR: Alert Manager initialization failed.");
    }

    // Initialize event logger
    if (!eventLoggerBegin())
    {
        Serial.println("ERROR: Event Logger initialization failed.");
    }

    // Record system startup
    logEvent(EventType::SYSTEM_START);

    // Initialize sampling timer
    lastSampleTime = millis();

    Serial.println("System initialization complete.");
}


// ---------------------------------------------------------
// Main Monitoring Loop
// ---------------------------------------------------------

void loop()
{
    unsigned long currentTime = millis();

    /*
     * Execute monitoring only when the configured
     * sampling interval has elapsed.
     */
    if (currentTime - lastSampleTime >=
        systemConfig.samplingIntervalMs)
    {
        lastSampleTime = currentTime;

        SensorData sensorData;

        // Read sensor
        bool sensorReadSuccessful =
            sensorManagerRead(sensorData);

        // -------------------------------------------------
        // Sensor Failure Handling
        // -------------------------------------------------

        if (!sensorReadSuccessful || !sensorData.valid)
        {
            Serial.println("ERROR: Invalid sensor data.");

            // Show sensor fault
            showSensorFault();

            // Log failure only when entering fault state
            if (previousSensorValid)
            {
                logEvent(EventType::SENSOR_FAILURE);
            }

            previousSensorValid = false;

            return;
        }

        // -------------------------------------------------
        // Sensor Recovery
        // -------------------------------------------------

        if (!previousSensorValid)
        {
            logEvent(EventType::SENSOR_RECOVERY);

            previousSensorValid = true;
        }

        // -------------------------------------------------
        // Condition Classification
        // -------------------------------------------------

        ConditionState previousState = currentState;

        currentState = classifyCondition(
            sensorData.temperature,
            sensorData.humidity,
            previousState
        );

        // -------------------------------------------------
        // Detect State Changes
        // -------------------------------------------------

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

        // -------------------------------------------------
        // Update Local Alerts
        // -------------------------------------------------

        updateAlerts(
            currentState,
            sensorData.temperature,
            sensorData.humidity
        );

        // -------------------------------------------------
        // Debug Information
        // -------------------------------------------------

        Serial.print("Current State: ");
        Serial.println(
            conditionStateToString(currentState)
        );
    }
}
