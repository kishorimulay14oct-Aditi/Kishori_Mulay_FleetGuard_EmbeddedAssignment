#ifndef ALERT_MANAGER_H
#define ALERT_MANAGER_H

#include "condition_classifier.h"

/*
 * FleetGuard Alert Manager
 *
 * Responsible for local indication of system condition:
 * - OLED display
 * - RGB/status LED
 * - Buzzer
 */

bool alertManagerBegin();

/**
 * Update local alerts according to the current condition.
 *
 * @param state Current environmental condition.
 * @param temperature Current temperature.
 * @param humidity Current relative humidity.
 */
void updateAlerts(
    ConditionState state,
    float temperature,
    float humidity
);

/**
 * Display a sensor/system fault condition.
 */
void showSensorFault();

#endif
