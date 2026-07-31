#include "condition_classifier.h"
#include "config.h"

/*
 * FleetGuard Condition Classifier
 *
 * Classification is based on both temperature and humidity.
 *
 * Hysteresis prevents rapid state changes when sensor readings
 * fluctuate around threshold values.
 *
 * Engineering demonstration values are defined in config.h.
 */

ConditionState classifyCondition(
    float temperature,
    float humidity,
    ConditionState previousState)
{
    /*
     * CRITICAL STATE
     *
     * If either temperature OR humidity reaches the critical
     * entry threshold, the system enters CRITICAL.
     */
    if (temperature >= TEMPERATURE_CRITICAL_ENTER ||
        humidity >= HUMIDITY_CRITICAL_ENTER)
    {
        return ConditionState::CRITICAL;
    }

    /*
     * If the system is already CRITICAL, do not immediately
     * leave the state when the value drops slightly.
     *
     * Both parameters must be below their critical exit
     * thresholds before returning to WARNING.
     */
    if (previousState == ConditionState::CRITICAL)
    {
        if (temperature > TEMPERATURE_CRITICAL_EXIT ||
            humidity > HUMIDITY_CRITICAL_EXIT)
        {
            return ConditionState::CRITICAL;
        }

        return ConditionState::WARNING;
    }

    /*
     * WARNING STATE
     *
     * If either parameter reaches the warning entry threshold,
     * enter WARNING.
     */
    if (temperature >= TEMPERATURE_WARNING_ENTER ||
        humidity >= HUMIDITY_WARNING_ENTER)
    {
        return ConditionState::WARNING;
    }

    /*
     * If the system is already WARNING, require both parameters
     * to fall below their warning exit thresholds before
     * returning to NORMAL.
     */
    if (previousState == ConditionState::WARNING)
    {
        if (temperature > TEMPERATURE_WARNING_EXIT ||
            humidity > HUMIDITY_WARNING_EXIT)
        {
            return ConditionState::WARNING;
        }

        return ConditionState::NORMAL;
    }

    /*
     * If no warning or critical threshold is active,
     * the condition is NORMAL.
     */
    return ConditionState::NORMAL;
}


const char* conditionStateToString(ConditionState state)
{
    switch (state)
    {
        case ConditionState::NORMAL:
            return "NORMAL";

        case ConditionState::WARNING:
            return "WARNING";

        case ConditionState::CRITICAL:
            return "CRITICAL";

        default:
            return "UNKNOWN";
    }
}
