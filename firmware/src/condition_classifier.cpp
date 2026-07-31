#include "condition_classifier.h"
#include "config.h"

/*
 * FleetGuard Condition Classifier
 *
 * Classification is based on temperature and relative humidity.
 *
 * Either parameter can cause the system to move to a more
 * severe condition.
 *
 * Hysteresis prevents rapid state changes when sensor values
 * fluctuate around threshold boundaries.
 *
 * Engineering demonstration values are defined in config.h.
 */


ConditionState classifyCondition(
    float temperature,
    float humidity,
    ConditionState previousState)
{
    // -----------------------------------------------------
    // CRITICAL STATE
    // -----------------------------------------------------

    /*
     * Enter CRITICAL if either temperature or humidity
     * reaches its critical entry threshold.
     */
    if (temperature >= TEMPERATURE_CRITICAL_ENTER ||
        humidity >= HUMIDITY_CRITICAL_ENTER)
    {
        return ConditionState::CRITICAL;
    }


    /*
     * If already CRITICAL, remain CRITICAL until BOTH
     * temperature and humidity have recovered below their
     * respective critical exit thresholds.
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


    // -----------------------------------------------------
    // WARNING STATE
    // -----------------------------------------------------

    /*
     * Enter WARNING if either temperature or humidity
     * reaches its warning entry threshold.
     */
    if (temperature >= TEMPERATURE_WARNING_ENTER ||
        humidity >= HUMIDITY_WARNING_ENTER)
    {
        return ConditionState::WARNING;
    }


    /*
     * If already WARNING, remain WARNING until BOTH
     * temperature and humidity have recovered below their
     * respective warning exit thresholds.
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


    // -----------------------------------------------------
    // NORMAL STATE
    // -----------------------------------------------------

    return ConditionState::NORMAL;
}


// ---------------------------------------------------------
// Condition State String
// ---------------------------------------------------------

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
