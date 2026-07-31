#include "alert_manager.h"
#include "config.h"
#include <Arduino.h>

/*
 * FleetGuard Alert Manager
 *
 * Handles:
 * - RGB/status LED
 * - Buzzer
 * - Local display interface
 *
 * The OLED implementation is kept modular so that the exact
 * display library can be added after the final hardware is selected.
 */

// ---------------------------------------------------------
// Internal helper functions
// ---------------------------------------------------------

static void setLed(bool red, bool green, bool blue)
{
    digitalWrite(LED_RED_PIN, red ? HIGH : LOW);
    digitalWrite(LED_GREEN_PIN, green ? HIGH : LOW);
    digitalWrite(LED_BLUE_PIN, blue ? HIGH : LOW);
}

static void buzzerOff()
{
    digitalWrite(BUZZER_PIN, LOW);
}

static void buzzerOn()
{
    digitalWrite(BUZZER_PIN, HIGH);
}


// ---------------------------------------------------------
// Initialization
// ---------------------------------------------------------

bool alertManagerBegin()
{
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);

    pinMode(BUZZER_PIN, OUTPUT);

    // Start in a safe inactive state
    setLed(false, false, false);
    buzzerOff();

    Serial.println("Alert Manager initialized.");

    return true;
}


// ---------------------------------------------------------
// Alert Update
// ---------------------------------------------------------

void updateAlerts(
    ConditionState state,
    float temperature,
    float humidity)
{
    switch (state)
    {
        case ConditionState::NORMAL:

            // Green LED
            setLed(false, true, false);

            // No audible alert
            buzzerOff();

            Serial.println("STATUS: NORMAL");

            break;


        case ConditionState::WARNING:

            // Yellow = Red + Green
            setLed(true, true, false);

            /*
             * Simple demonstration alert.
             *
             * In the final implementation this can be replaced
             * with a non-blocking periodic buzzer pattern.
             */
            buzzerOn();

            Serial.println("STATUS: WARNING");

            break;


        case ConditionState::CRITICAL:

            // Red LED
            setLed(true, false, false);

            // Critical audible alert
            buzzerOn();

            Serial.println("STATUS: CRITICAL");

            break;
    }

    /*
     * Temporary serial display.
     *
     * The actual OLED implementation will be connected here
     * after the final OLED library and model are selected.
     */
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
}


// ---------------------------------------------------------
// Sensor Fault
// ---------------------------------------------------------

void showSensorFault()
{
    /*
     * Fault indication:
     * Red LED + buzzer.
     */
    setLed(true, false, false);

    buzzerOn();

    Serial.println("STATUS: SENSOR FAULT");
}
