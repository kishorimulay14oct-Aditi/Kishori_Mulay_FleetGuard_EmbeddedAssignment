#include "event_logger.h"

#include <Arduino.h>
#include <Preferences.h>

/*
 * FleetGuard Event Logger
 *
 * Events are stored in ESP32 NVS so that important events
 * remain available after a power restart.
 *
 * Prototype storage strategy:
 * - Store the latest event
 * - Store total event count
 *
 * Future production implementation may use:
 * - Circular event buffer
 * - External flash
 * - SD card
 * - Cloud synchronization
 */

static Preferences eventStorage;

static bool loggerInitialized = false;


// ---------------------------------------------------------
// Initialization
// ---------------------------------------------------------

bool eventLoggerBegin()
{
    if (!eventStorage.begin("events", false))
    {
        Serial.println("ERROR: Event storage initialization failed.");

        loggerInitialized = false;

        return false;
    }

    loggerInitialized = true;

    Serial.println("Event Logger initialized.");

    return true;
}


// ---------------------------------------------------------
// Log Event
// ---------------------------------------------------------

void logEvent(EventType event)
{
    if (!loggerInitialized)
    {
        return;
    }

    const char* eventText =
        eventTypeToString(event);

    /*
     * Read current event count.
     */
    uint32_t eventCount =
        eventStorage.getUInt("count", 0);

    eventCount++;

    /*
     * Store the latest event.
     */
    eventStorage.putString(
        "last_event",
        eventText
    );

    /*
     * Store event count.
     */
    eventStorage.putUInt(
        "count",
        eventCount
    );

    /*
     * Print event information for debugging.
     */
    Serial.print("[EVENT #");
    Serial.print(eventCount);
    Serial.print("] ");
    Serial.println(eventText);
}


// ---------------------------------------------------------
// Event Type to String
// ---------------------------------------------------------

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
