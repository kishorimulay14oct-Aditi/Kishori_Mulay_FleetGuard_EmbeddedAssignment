# FleetGuard Firmware

This directory contains the embedded firmware for the FleetGuard
Intelligent Refrigerated Container Monitoring System.

## Firmware Responsibilities

The firmware is responsible for:

- Temperature and humidity acquisition
- Sensor data validation
- Normal/Warning/Critical classification
- Threshold hysteresis
- Local OLED display
- RGB LED indication
- Buzzer alerts
- Event logging
- Configuration management
- Sensor failure handling
- Power restart handling
- Future Wi-Fi/MQTT connectivity

## Firmware Architecture

The firmware is divided into independent modules:

- Sensor Manager
- Condition Classifier
- Alert Manager
- Event Logger
- Configuration Manager
- Main Application

## Development Environment

The initial proof-of-concept can use an ESP32-compatible Arduino
environment.

PlatformIO may be used for a structured project build.

## Future Development

Future versions may migrate toward ESP-IDF and RTOS-based task
management when more advanced networking, security, and resource
management are required.
