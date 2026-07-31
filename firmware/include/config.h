#ifndef FLEETGUARD_CONFIG_H
#define FLEETGUARD_CONFIG_H

/*
 * FleetGuard Configuration
 *
 * Engineering demonstration values only.
 * These values must be replaced with application-specific
 * limits before production deployment.
 */

// Sampling interval
#define DEFAULT_SAMPLING_INTERVAL_MS 5000UL

// Temperature thresholds (°C)
#define TEMPERATURE_WARNING_ENTER 8.0f
#define TEMPERATURE_WARNING_EXIT  7.5f

#define TEMPERATURE_CRITICAL_ENTER 10.0f
#define TEMPERATURE_CRITICAL_EXIT  9.5f

// Humidity thresholds (%RH)
#define HUMIDITY_WARNING_ENTER 70.0f
#define HUMIDITY_WARNING_EXIT  65.0f

#define HUMIDITY_CRITICAL_ENTER 80.0f
#define HUMIDITY_CRITICAL_EXIT  75.0f

// Sensor validation limits
#define MIN_VALID_TEMPERATURE -40.0f
#define MAX_VALID_TEMPERATURE 85.0f

#define MIN_VALID_HUMIDITY 0.0f
#define MAX_VALID_HUMIDITY 100.0f

// I2C pins for environmental sensor and OLED
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// SHT31 I2C address
#define SHT31_I2C_ADDRESS 0x44

#define LED_RED_PIN 25
#define LED_GREEN_PIN 26
#define LED_BLUE_PIN 27

#define BUZZER_PIN 14

#define DOOR_SENSOR_PIN 33

#define UART_TX_PIN 17
#define UART_RX_PIN 16

Instead of writing:

if (temperature >= 8.0)

throughout the firmware, we'll eventually use:

TEMPERATURE_WARNING_ENTER

This gives you one place to change the engineering parameters.

It also makes the hysteresis very clear:

WARNING ENTER = 8.0°C
WARNING EXIT  = 7.5°C

CRITICAL ENTER = 10.0°C
CRITICAL EXIT  = 9.5°C

So the system won't rapidly switch:

NORMAL ↔ WARNING ↔ NORMAL ↔ WARNING

when the sensor fluctuates around 8°C.

#endif
