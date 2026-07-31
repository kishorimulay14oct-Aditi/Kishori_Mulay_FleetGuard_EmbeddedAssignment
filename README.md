# FleetGuard Intelligent Refrigerated Container Monitoring System

## Project Overview

FleetGuard is an ESP32-based edge monitoring system designed for refrigerated transport containers carrying temperature-sensitive products such as pharmaceuticals, vaccines, laboratory samples, and specialty chemicals.

The system continuously monitors **temperature and relative humidity**, validates sensor readings, classifies environmental conditions as **NORMAL, WARNING, or CRITICAL**, generates local alerts, and records important events.

The system follows a **local-first architecture**, allowing critical monitoring and alerting functions to continue even when network connectivity is unavailable.

Future Wi-Fi and MQTT connectivity can be added for cloud-based fleet monitoring.

---

## Problem Statement

Manual or intermittent environmental monitoring can result in:

* Delayed detection of unsafe conditions
* Product spoilage
* Regulatory compliance risks
* Limited historical information
* Increased operational costs

FleetGuard addresses these challenges through continuous local monitoring and immediate detection of abnormal environmental conditions.

---

## Project Objectives

The main objectives are:

* Monitor temperature continuously
* Monitor relative humidity
* Validate sensor readings
* Detect invalid or failed sensor readings
* Classify environmental conditions
* Generate local alerts
* Maintain local event information
* Support configurable operating parameters
* Handle sensor failures and recovery
* Continue operating during network unavailability
* Provide a security-aware architecture
* Support future Wi-Fi and MQTT/cloud integration
* Provide a scalable architecture for fleet deployment

---

## Proposed Solution

The proposed system uses an **ESP32 edge controller** connected to environmental sensors and local alerting interfaces.

The ESP32 performs:

1. Sensor acquisition
2. Sensor data validation
3. Condition classification
4. Hysteresis processing
5. Alert generation
6. Event logging
7. Configuration management
8. Local display and status indication

Future connectivity can be added through Wi-Fi and MQTT without affecting the core local monitoring functions.

---

## High-Level Architecture

![FleetGuard System Architecture](https://github.com/user-attachments/assets/6b08067a-7467-43c7-bbe7-3a3ec842a1fb)

```text
        Temperature / Humidity Sensor
                    |
                    v
             +-------------+
             |    ESP32    |
             | Edge        |
             | Controller  |
             +------+------+
                    |
        +-----------+-----------+-------------+
        |           |           |             |
        v           v           v             v
   Sensor       Condition     Alert        Event
   Manager     Classifier    Manager       Logger
                    |           |             |
                    |       +---+---+         v
                    |       |   |   |      NVS
                    |       v   v   v
                    |      OLED LED Buzzer
                    |
                    v
              System State

          Future Connectivity
                    |
                  Wi-Fi
                    |
                  MQTT
                    |
              Cloud / Dashboard
```

---

## Operating Conditions

FleetGuard classifies the environmental condition into three primary states.

### NORMAL

All monitored parameters are within the configured safe operating range.

### WARNING

One or more parameters have crossed a warning threshold and require attention.

### CRITICAL

One or more parameters have exceeded a configured critical threshold and require immediate attention.

The threshold values used in the proof-of-concept are **engineering demonstration values**. Application-specific limits must be defined before production deployment.

---

## Hysteresis

Hysteresis is used to prevent rapid state switching when sensor values fluctuate around a threshold.

Example:

```text
Temperature >= 8.0°C

NORMAL ------------------> WARNING
  ^                           |
  |                           |
  +---------------------------+
       Temperature <= 7.5°C
```

The system enters WARNING at 8.0°C but returns to NORMAL only after the temperature falls to 7.5°C or below.

Similarly, critical-state hysteresis is used to prevent rapid switching between WARNING and CRITICAL.

The hysteresis values are configurable engineering demonstration values.

---

## Main Features

* Temperature monitoring
* Relative humidity monitoring
* Sensor data validation
* Sensor failure detection
* Sensor recovery detection
* NORMAL/WARNING/CRITICAL classification
* Hysteresis-based state transitions
* OLED display
* RGB/status LED indication
* Buzzer alerts
* Event logging
* ESP32 NVS storage
* Configurable sampling interval
* Configurable environmental thresholds
* Power restart handling
* Future Wi-Fi/MQTT readiness
* Security-aware architecture
* Scalable fleet architecture

---

## Hardware

The prototype consists of:

* ESP32 microcontroller
* Temperature and humidity sensor
* OLED display
* RGB/status LED
* Buzzer
* Power supply subsystem
* Optional door-status sensor

Detailed hardware connections and pin assignments are documented in:

`hardware/pin_mapping.md`

---

## Firmware Architecture

The firmware is organized into modular components:

```text
firmware/
├── include/
│   └── config.h
└── src/
    ├── main.cpp
    ├── sensor_manager.cpp
    ├── condition_classifier.cpp
    ├── alert_manager.cpp
    ├── event_logger.cpp
    └── configuration_manager.cpp
```

The main functional modules are:

### Sensor Manager

Responsible for sensor initialization, measurement acquisition, validation, and sensor fault detection.

### Condition Classifier

Determines the current environmental state:

```text
NORMAL
WARNING
CRITICAL
```

and applies hysteresis to provide stable state transitions.

### Alert Manager

Controls local notification through:

* OLED
* RGB/status LED
* Buzzer

### Event Logger

Records important events using ESP32 non-volatile storage.

The current prototype stores the event count and latest event in NVS.

### Configuration Manager

Maintains configurable system parameters such as sampling intervals and environmental thresholds.

---

## Event Logging

Important events include:

* SYSTEM_START
* SENSOR_FAILURE
* SENSOR_RECOVERY
* NORMAL_TO_WARNING
* WARNING_TO_CRITICAL
* CRITICAL_TO_WARNING
* WARNING_TO_NORMAL
* CONFIGURATION_CHANGE
* POWER_RESTART

The prototype uses ESP32 NVS for persistent local storage.

```text
Event
  |
  v
Event Logger
  |
  +---- Event Count
  |
  +---- Latest Event
  |
  v
ESP32 NVS
```

The current implementation is a prototype storage strategy. A production implementation may use a circular event buffer, external flash, SD card, or cloud synchronization.

---

## Fault Handling

Invalid or unavailable sensor data is not treated as a valid environmental measurement.

```text
Sensor Reading
      |
      v
  Data Valid?
    /     \
  YES      NO
   |        |
   v        v
Classification
             |
             v
       Sensor Failure
          /       \
         v         v
      Alert     Event Log
```

The system provides a clear local fault indication when sensor operation fails.

---

## Local User Interface

### OLED

The display can provide:

* Temperature
* Relative humidity
* Current condition
* Sensor status
* Warning messages
* Critical messages
* Fault information

### RGB/Status LED

```text
GREEN   → NORMAL
YELLOW  → WARNING
RED     → CRITICAL
FAULT   → SENSOR / SYSTEM ERROR
```

### Buzzer

The buzzer provides audible notification for warning, critical, and fault conditions.

---

## Build and Run

### Requirements

* ESP32 development board
* PlatformIO
* Visual Studio Code
* Required environmental sensor
* OLED display
* RGB/status LED
* Buzzer

### Configuration

System configuration parameters are maintained in:

```text
firmware/include/config.h
```

Hardware pin assignments are documented in:

```text
hardware/pin_mapping.md
```

### Build

Open the project in Visual Studio Code with PlatformIO and build the firmware.

The firmware has been verified using the PlatformIO build process.

### Upload

Connect the ESP32 to the development computer and upload the firmware using PlatformIO.

### Serial Monitoring

The serial monitor can be used to observe system startup, sensor status, state transitions, and event logging information.

---

## Testing

Testing documentation is available in:

```text
docs/testing.md
```

The test coverage includes:

* System startup
* Normal sensor operation
* Sensor failure
* Sensor recovery
* NORMAL → WARNING transition
* WARNING → CRITICAL transition
* CRITICAL → WARNING transition
* WARNING → NORMAL transition
* Configuration handling
* Event logging
* Power restart
* Firmware build verification

---

## Repository Structure

```text
FleetGuard/
│
├── README.md
│
├── firmware/
│   ├── include/
│   │   └── config.h
│   └── src/
│       ├── main.cpp
│       ├── sensor_manager.cpp
│       ├── condition_classifier.cpp
│       ├── alert_manager.cpp
│       ├── event_logger.cpp
│       └── configuration_manager.cpp
│
├── hardware/
│   └── pin_mapping.md
│
├── docs/
│   ├── testing.md
│   └── system_architecture.md
│
├── diagrams/
│   └── [project diagrams]
│
└── platformio.ini
```

---

## Security Considerations

Security is considered as part of the system architecture.

A production implementation should consider:

* Unique device identity
* Secure device provisioning
* Authenticated communication
* TLS encryption
* Secure configuration
* Firmware integrity verification
* Secure firmware update mechanism
* Access control

Future MQTT communication should use authenticated and encrypted connections.

---

## Network Independence

The core monitoring functions are designed to operate locally.

```text
Sensor
  |
  v
ESP32
  |
  v
Validation
  |
  v
Classification + Hysteresis
  |
  +----> OLED / LED / Buzzer
  |
  +----> Event Logging
  |
  v
Local Operation
       |
       | Future Connectivity
       v
Wi-Fi → MQTT → Cloud → Dashboard
```

If Wi-Fi or MQTT connectivity becomes unavailable, the core local monitoring, classification, alerting, and event logging functions can continue operating.

---

## Future Enhancements

Potential production enhancements include:

* Circular event history buffer
* External flash or SD-card storage
* Wi-Fi connectivity
* MQTT communication
* Cloud-based fleet dashboard
* Remote configuration
* Historical data visualization
* Device health monitoring
* Secure OTA firmware updates
* TLS-based communication
* Secure boot and firmware integrity verification
* Advanced analytics and predictive maintenance
* Multi-device fleet management

---

## Current Limitations

This repository represents an **edge-monitoring prototype**.

Current limitations include:

* Event logging currently stores the latest event and total event count rather than a complete historical event database.
* Cloud/MQTT functionality is future scope.
* Threshold values are engineering demonstration values.
* Production security mechanisms require further implementation.
* Extensive long-term event storage may require external storage.

---

## Project Status

```text
Hardware Documentation       ✅ Complete
Configuration                ✅ Complete
Firmware Integration         ✅ Complete
PlatformIO Build             ✅ Verified
Testing Documentation        ✅ Complete
System Architecture          ✅ Complete
README                       ✅ Complete
Final Repository Review      🔄 Pending
```

---

## Conclusion

FleetGuard provides a modular ESP32-based architecture for local monitoring of environmental conditions in refrigerated containers.

The system validates sensor data, classifies environmental conditions using configurable thresholds and hysteresis, provides local alerts, and records important events using non-volatile storage.

The local-first architecture ensures that core monitoring functions remain independent of network connectivity while providing a foundation for future Wi-Fi, MQTT, cloud, security, and fleet-management capabilities.

The repository is designed to provide a clear foundation for further development toward a production-oriented refrigerated container monitoring solution.
