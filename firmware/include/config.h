#ifndef FLEETGUARD_CONFIG_H
#define FLEETGUARD_CONFIG_H

/*
 * FleetGuard Configuration
 *
 * Engineering demonstration values only.
 * These values must be replaced with application-specific
 * limits before production deployment.
 */

/* =========================
 * Sampling Configuration
 * ========================= */

#define DEFAULT_SAMPLING_INTERVAL_MS 5000UL


/* =========================
 * Temperature Thresholds
 * ========================= */

/*
 * Hysteresis is used to prevent rapid state switching when
 * temperature fluctuates around a threshold.
 */

#define TEMPERATURE_WARNING_ENTER   8.0f
#define TEMPERATURE_WARNING_EXIT    7.5f

#define TEMPERATURE_CRITICAL_ENTER 10.0f
#define TEMPERATURE_CRITICAL_EXIT   9.5f


/* =========================
 * Humidity Thresholds
 * ========================= */

/*
 * Hysteresis is also applied to relative humidity.
 */

#define HUMIDITY_WARNING_ENTER   70.0f
#define HUMIDITY_WARNING_EXIT    65.0f

#define HUMIDITY_CRITICAL_ENTER  80.0f
#define HUMIDITY_CRITICAL_EXIT   75.0f


/* =========================
 * Sensor Validation Limits
 * ========================= */

#define MIN_VALID_TEMPERATURE -40.0f
#define MAX_VALID_TEMPERATURE  85.0f

#define MIN_VALID_HUMIDITY      0.0f
#define MAX_VALID_HUMIDITY    100.0f


/* =========================
 * I2C Configuration
 * ========================= */

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

#define SHT31_I2C_ADDRESS 0x44

#define OLED_I2C_ADDRESS 0x3C
#define OLED_WIDTH 128
#define OLED_HEIGHT 64


/* =========================
 * Local Alert Outputs
 * ========================= */

#define LED_RED_PIN   25
#define LED_GREEN_PIN 26
#define LED_BLUE_PIN  27

#define BUZZER_PIN 14


/* =========================
 * Optional Door Sensor
 * ========================= */

#define DOOR_SENSOR_PIN 33


/* =========================
 * UART Debug Interface
 * ========================= */

#define UART_TX_PIN 17
#define UART_RX_PIN 16

#endif /* FLEETGUARD_CONFIG_H */
