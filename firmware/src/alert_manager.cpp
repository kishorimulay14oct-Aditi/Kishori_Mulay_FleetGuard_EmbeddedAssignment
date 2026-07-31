#include "alert_manager.h"
#include "config.h"
#include "condition_classifier.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*
 * FleetGuard Alert Manager
 *
 * Responsibilities:
 * - OLED display
 * - RGB/status LED
 * - Buzzer
 * - Local environmental status indication
 */

static Adafruit_SSD1306 display(
    OLED_WIDTH,
    OLED_HEIGHT,
    &Wire,
    -1
);

static bool displayInitialized = false;


// ---------------------------------------------------------
// RGB LED
// ---------------------------------------------------------

static void setLed(bool red, bool green, bool blue)
{
    digitalWrite(LED_RED_PIN, red ? HIGH : LOW);
    digitalWrite(LED_GREEN_PIN, green ? HIGH : LOW);
    digitalWrite(LED_BLUE_PIN, blue ? HIGH : LOW);
}


// ---------------------------------------------------------
// Buzzer
// ---------------------------------------------------------

static void buzzerOff()
{
    digitalWrite(BUZZER_PIN, LOW);
}


static void buzzerOn()
{
    digitalWrite(BUZZER_PIN, HIGH);
}


// ---------------------------------------------------------
// OLED Display
// ---------------------------------------------------------

static void updateDisplay(
    ConditionState state,
    float temperature,
    float humidity)
{
    if (!displayInitialized)
    {
        return;
    }

    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    // Title
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("FleetGuard");

    // Temperature
    display.setCursor(0, 16);
    display.print("Temp: ");
    display.print(temperature, 1);
    display.println(" C");

    // Humidity
    display.setCursor(0, 28);
    display.print("Hum:  ");
    display.print(humidity, 1);
    display.println(" %");

    // Status
    display.setCursor(0, 44);
    display.print("Status: ");

    display.println(conditionStateToString(state));

    display.display();
}


static void displaySensorFault()
{
    if (!displayInitialized)
    {
        return;
    }

    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);

    display.setCursor(0, 0);
    display.println("FleetGuard");

    display.setTextSize(2);

    display.setCursor(0, 25);
    display.println("SENSOR");

    display.setCursor(0, 45);
    display.println("FAULT");

    display.display();
}


// ---------------------------------------------------------
// Initialization
// ---------------------------------------------------------

bool alertManagerBegin()
{
    // RGB LED
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);

    // Buzzer
    pinMode(BUZZER_PIN, OUTPUT);

    // Safe initial state
    setLed(false, false, false);
    buzzerOff();

    // Initialize OLED
    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_I2C_ADDRESS))
    {
        Serial.println("WARNING: OLED not detected.");

        displayInitialized = false;
    }
    else
    {
        displayInitialized = true;

        display.clearDisplay();

        display.setTextColor(SSD1306_WHITE);

        display.setTextSize(1);
        display.setCursor(0, 0);

        display.println("FleetGuard");
        display.println();
        display.println("Initializing...");

        display.display();

        Serial.println("OLED initialized.");
    }

    Serial.println("Alert Manager initialized.");

    return true;
}


// ---------------------------------------------------------
// Update Alerts
// ---------------------------------------------------------

void updateAlerts(
    ConditionState state,
    float temperature,
    float humidity)
{
    switch (state)
    {
        case ConditionState::NORMAL:

            // Green
            setLed(false, true, false);

            // No buzzer
            buzzerOff();

            Serial.println("STATUS: NORMAL");

            break;


        case ConditionState::WARNING:

            // Yellow = Red + Green
            setLed(true, true, false);

            // Warning buzzer
            buzzerOn();

            Serial.println("STATUS: WARNING");

            break;


        case ConditionState::CRITICAL:

            // Red
            setLed(true, false, false);

            // Critical buzzer
            buzzerOn();

            Serial.println("STATUS: CRITICAL");

            break;
    }

    // Update OLED
    updateDisplay(
        state,
        temperature,
        humidity
    );
}


// ---------------------------------------------------------
// Sensor Fault
// ---------------------------------------------------------

void showSensorFault()
{
    // Red LED
    setLed(true, false, false);

    // Buzzer
    buzzerOn();

    // OLED
    displaySensorFault();

    Serial.println("STATUS: SENSOR FAULT");
}
