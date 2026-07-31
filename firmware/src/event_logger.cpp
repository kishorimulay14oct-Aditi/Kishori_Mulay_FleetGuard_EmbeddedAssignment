#include "event_logger.h"
#include <Arduino.h>

/*
 * FleetGuard Event Logger
 *
 * Prototype implementation:
 * Events are currently written to the serial console.
 *
 * Later, this module can be connected to ESP32 NVS/flash
 * for persistent non-volatile event storage.
 */

static bool loggerInitialized = false;


bool eventLoggerBegin()
{
    loggerInitialized = true;

    Serial.println("Event Logger initialized.");

    return loggerInitialized;
}


void logEvent(EventType event)
{
    if (!loggerInitialized)
    {
        return;
    }

    Serial.print("[EVENT] ");
    Serial.println(eventTypeToString(event));

    /*
     * Future implementation:
     *
     * Store the event in ESP32 NVS/flash.
     *
     * Example:
     * - Timestamp
     * - Event type
     * - Temperature
     * - Humidity
     * - Device status
     */
}


const char* eventTypeToString(EventType event)
{
    switch (event)
    {
        case EventType::SYSTEM_START:
            return "SYSTEM_START";

        case EventType::SENSOR_FAILURE:
            return "SENSOR_FAILURE";

        case EventType::SENSOR_RECOVERY:
            return "SENSOR_RECOVERY";

        case EventType::NORMAL_TO_WARNING:
            return "NORMAL_TO_WARNING";

        case EventType::WARNING_TO_CRITICAL:
            return "WARNING_TO_CRITICAL";

        case EventType::CRITICAL_TO_WARNING:
            return "CRITICAL_TO_WARNING";

        case EventType::WARNING_TO_NORMAL:
            return "WARNING_TO_NORMAL";

        case EventType::CONFIGURATION_CHANGE:
            return "CONFIGURATION_CHANGE";

        case EventType::POWER_RESTART:
            return "POWER_RESTART";

        default:
            return "UNKNOWN_EVENT";
    }
}
