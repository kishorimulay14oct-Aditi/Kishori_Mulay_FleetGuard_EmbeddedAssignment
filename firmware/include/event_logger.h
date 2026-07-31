#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

/*
 * FleetGuard Event Logger
 *
 * Responsible for recording important system events such as:
 * - System startup
 * - Sensor failure/recovery
 * - Condition changes
 * - Configuration changes
 * - Power restart
 */

enum class EventType
{
    SYSTEM_START,
    SENSOR_FAILURE,
    SENSOR_RECOVERY,
    NORMAL_TO_WARNING,
    WARNING_TO_CRITICAL,
    CRITICAL_TO_WARNING,
    WARNING_TO_NORMAL,
    CONFIGURATION_CHANGE,
    POWER_RESTART
};

/**
 * Initialize the event logger.
 */
bool eventLoggerBegin();

/**
 * Record an event.
 *
 * @param event Event type to record.
 */
void logEvent(EventType event);

/**
 * Convert event type to readable text.
 */
const char* eventTypeToString(EventType event);

#endif
