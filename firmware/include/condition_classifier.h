#ifndef CONDITION_CLASSIFIER_H
#define CONDITION_CLASSIFIER_H

/*
 * FleetGuard Condition Classifier
 *
 * Determines the environmental condition:
 * NORMAL
 * WARNING
 * CRITICAL
 *
 * Hysteresis is used to prevent rapid state switching
 * around threshold boundaries.
 */

enum class ConditionState
{
    NORMAL,
    WARNING,
    CRITICAL
};

/**
 * Classify the current environmental condition.
 *
 * @param temperature Current temperature in °C
 * @param humidity Current relative humidity in %RH
 * @param previousState Previous system condition
 *
 * @return Current condition state
 */
ConditionState classifyCondition(
    float temperature,
    float humidity,
    ConditionState previousState
);

/**
 * Convert condition state to readable text.
 */
const char* conditionStateToString(ConditionState state);
We are separating the interface from the implementation.

Think of it as:

condition_classifier.h
        |
        | tells the rest of firmware
        | what functions/types are available
        v
condition_classifier.cpp
        |
        | contains the actual logic
        v
Normal / Warning / Critical
#endif
