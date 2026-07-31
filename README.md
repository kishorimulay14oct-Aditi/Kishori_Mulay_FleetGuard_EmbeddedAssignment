# FleetGuard Intelligent Refrigerated Container Monitoring System

## Project Overview

FleetGuard Technologies operates refrigerated transport containers used
for transporting temperature-sensitive products such as pharmaceuticals,
vaccines, laboratory samples, and specialty chemicals.

This project presents an intelligent edge monitoring device designed to
continuously monitor environmental conditions inside refrigerated
containers.

The proposed system collects temperature and relative humidity data,
validates sensor readings, classifies operating conditions as Normal,
Warning, or Critical, generates local alerts, maintains an event history,
and provides an architecture ready for future cloud connectivity.

## Problem Statement

Manual environmental monitoring can result in delayed detection of
unsafe conditions, product spoilage, regulatory compliance risks,
limited historical data, and increased operational costs.

The proposed FleetGuard edge device addresses these problems by providing
continuous local monitoring and rapid detection of abnormal environmental
conditions.

## Project Objectives

The main objectives of this project are:

- Monitor temperature continuously
- Monitor relative humidity
- Validate sensor readings
- Detect invalid or failed sensor readings
- Classify environmental conditions as Normal, Warning, or Critical
- Generate local alerts
- Maintain a local event history
- Support configurable operating parameters
- Provide reliability during temporary sensor and communication failures
- Provide security-aware architecture
- Support future Wi-Fi and MQTT/cloud integration
- Provide a scalable architecture suitable for large fleet deployment

## Proposed Solution

The proposed solution uses an ESP32-based edge controller connected to
environmental sensors and local alerting interfaces.

The ESP32 performs local data acquisition, validation, condition
classification, event generation, and alert management.

Future connectivity can be added through Wi-Fi and MQTT without affecting
the core local monitoring functionality.

## High-Level Architecture
<img width="1536" height="1024" alt="image" src="https://github.com/user-attachments/assets/6b08067a-7467-43c7-bbe7-3a3ec842a1fb" />

```text
 Temperature / Humidity Sensors
              |
              v
       +--------------+
       |    ESP32     |
       |              |
       | Sensor Read  |
       | Validation   |
       | Classification
       | Event Manager|
       | Configuration|
       +------+-------+
              |
       +------+-------+----------------+
       |              |                |
       v              v                v
   OLED Display    LED/Buzzer       Event Log
                                      |
                                      v
                              Future Wi-Fi/MQTT
## Operating Conditions

The system classifies environmental conditions into three states:

### Normal

All monitored parameters are within the configured safe operating range.

### Warning

One or more parameters are approaching an unsafe limit.

### Critical

One or more parameters exceed the configured critical threshold.

The threshold values used in the proof-of-concept are engineering
demonstration values and must be replaced with application-specific
limits before production deployment.

## Main Features

- Temperature monitoring
- Relative humidity monitoring
- Sensor data validation
- Normal/Warning/Critical classification
- Local LED indication
- Buzzer alert
- Local display
- Event logging
- Configurable sampling interval
- Configurable threshold values
- Sensor failure handling
- Power restart handling
- Future Wi-Fi/MQTT readiness
- Security-aware architecture

## Hardware

The proposed prototype uses:

- ESP32 microcontroller
- Temperature and humidity sensor
- OLED display
- RGB/status LED
- Buzzer
- Optional door-status sensor
- Power supply subsystem

The final component selection and engineering justification are documented
in the `hardware/` and `docs/` directories.
- Scalable device architecture

## Software

The firmware is organized into modular components:

- Sensor Manager
- Condition Classifier
- Alert Manager
- Event Logger
- Configuration Manager
- Main Application

The firmware architecture is documented in the `firmware/` directory.
                                      |
                                      v
                                   Cloud
