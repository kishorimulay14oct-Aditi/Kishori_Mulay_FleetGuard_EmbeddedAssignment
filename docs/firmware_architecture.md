# FleetGuard Firmware Architecture

## 1. Purpose

The FleetGuard firmware is responsible for monitoring environmental
conditions, validating sensor data, classifying the current operating
condition, generating local alerts, recording important events, and
managing configurable parameters.

The firmware is designed using modular components so that individual
functions can be developed, tested, and maintained independently.

---

## 2. Firmware Architecture

The firmware consists of the following major modules:

```text
+--------------------------------------------------+
|              Main Application                    |
|                  main.cpp                        |
+-------------------------+------------------------+
                          |
          +---------------+---------------+
          |               |               |
          v               v               v
+----------------+ +---------------+ +---------------+
| Sensor Manager | | Configuration | | Event Logger  |
|                | | Manager       | |               |
+-------+--------+ +---------------+ +---------------+
        |
        v
+------------------------+
| Data Validation        |
+-----------+------------+
            |
            v
+------------------------+
| Condition Classifier   |
| + Threshold Logic      |
| + Hysteresis           |
+-----------+------------+
            |
            v
+------------------------+
| Alert Manager          |
+-----+---------+--------+
      |         |
      v         v
    OLED     LED/Buzzer
3. Main Application

The Main Application coordinates the different firmware modules.

Typical responsibilities include:

System initialization
Module initialization
Main monitoring loop
Sampling scheduling
Calling sensor acquisition
Calling condition classification
Updating local indicators
Recording events
Handling communication services

The main application should not contain all functionality in a single
source file.

Instead, functionality shall be distributed across independent modules.

4. Sensor Manager

The Sensor Manager is responsible for interaction with the temperature
and humidity sensor.

Responsibilities
Sensor initialization
Sensor reading
Temperature acquisition
Humidity acquisition
Sensor communication status
Sensor failure detection
Sensor data validation

The Sensor Manager shall provide validated sensor data to the
Condition Classifier.

5. Data Validation

Sensor readings shall be validated before classification.

Validation may include:

Sensor communication success
Valid numeric reading
Temperature range check
Humidity range check
Detection of invalid values
Detection of unavailable readings

Invalid readings shall not be treated as normal environmental
conditions.

If a sensor failure is detected, the system shall generate an
appropriate fault indication and event.

6. Condition Classifier

The Condition Classifier determines the current environmental state.

The supported states are:

NORMAL
WARNING
CRITICAL

The classification uses configurable temperature and humidity
thresholds.

The classifier shall use the previous system state together with the
current sensor measurements.

7. Threshold Hysteresis

Hysteresis shall be implemented in the Condition Classifier to prevent
rapid switching between states when measurements fluctuate around a
threshold.

For example:

Enter WARNING:
Temperature >= 8.0°C

Return to NORMAL:
Temperature <= 7.5°C

For the critical state:

Enter CRITICAL:
Temperature >= 10.0°C

Return to WARNING:
Temperature <= 9.5°C

Example:

Temperature:
7.9 → 8.1 → 7.9 → 7.7 → 7.5°C

State:
NORMAL → WARNING → WARNING → WARNING → NORMAL

This prevents alarm/status chattering.

The hysteresis values shall be configurable.

8. Condition State Machine

The firmware shall maintain the current condition state.

Conceptually:

                 >= Warning Entry
        +------------------------------+
        |                              |
        v                              |
    +--------+                    +----------+
    | NORMAL | -----------------> | WARNING  |
    +--------+                    +----------+
        ^                              |
        |                              |
        | <= Warning Exit              | >= Critical Entry
        |                              |
        |                              v
        |                         +----------+
        +-------------------------| CRITICAL |
             <= Critical Exit     +----------+

The exact transitions shall be implemented using configurable
thresholds and hysteresis values.

9. Alert Manager

The Alert Manager converts the current system state into local
indications.

Normal
LED    → Green
Buzzer → OFF
OLED   → NORMAL
Warning
LED    → Yellow
Buzzer → Periodic Beep
OLED   → WARNING
Critical
LED    → Red
Buzzer → Repeated Alert
OLED   → CRITICAL
Sensor Fault
LED    → Fault indication
Buzzer → Fault pattern
OLED   → SENSOR ERROR
10. Event Logger

The Event Logger records important system events.

Examples include:

System startup
Sensor initialization
Sensor failure
Sensor recovery
NORMAL → WARNING transition
WARNING → CRITICAL transition
CRITICAL → WARNING transition
WARNING → NORMAL transition
Configuration change
Power restart
Communication status change

The event logger shall use local non-volatile storage for important
events.

11. Configuration Manager

The Configuration Manager manages configurable parameters.

Possible parameters include:

Sampling Interval
Temperature Warning Threshold
Temperature Critical Threshold
Temperature Hysteresis
Humidity Warning Threshold
Humidity Critical Threshold
Humidity Hysteresis
Alert Configuration

Configuration values should be stored in non-volatile memory so that they
can survive a power restart.

12. Sampling and Main Loop

The firmware shall periodically sample environmental data.

Conceptual sequence:

System Start
     |
     v
Initialize Hardware
     |
     v
Load Configuration
     |
     v
Initialize Sensor
     |
     v
Read Sensor
     |
     v
Validate Data
     |
     +---- Invalid ----> Fault Handling
     |
    Valid
     |
     v
Classify Condition
     |
     v
Apply Hysteresis
     |
     v
Update OLED / LED / Buzzer
     |
     v
Log Important Event
     |
     v
Wait for Next Sampling Interval
     |
     +--------------------------+
                                |
                                v
                         Repeat Monitoring
13. Sensor Failure Handling

If the sensor does not respond or provides invalid data:

Sensor Read
     |
     v
Data Valid?
   /     \
 NO       YES
 |         |
 v         v
Fault    Classification
Handling
 |
 +----> Alert
 |
 +----> Event Log
 |
 +----> OLED Error

The firmware shall ensure that an invalid sensor reading is not
incorrectly classified as NORMAL.

14. Power Restart Handling

After a power restart:

Power On
   |
   v
System Initialization
   |
   v
Load Configuration
   |
   v
Initialize Peripherals
   |
   v
Initialize Sensor
   |
   v
Validate Sensor
   |
   v
Start Monitoring
   |
   v
Log Restart Event

Stored configuration shall be retained across restarts.

15. Timing and Sampling

The sampling interval shall be configurable.

The initial proof-of-concept may use a periodic sampling interval.

The firmware should avoid unnecessary blocking delays where possible.

The architecture should allow future migration toward a task-based or
RTOS-based implementation if required.

16. Future Wi-Fi and MQTT

Network communication is separated from the core monitoring logic.

Future firmware functionality may include:

Sensor Data
     |
     v
FleetGuard ESP32
     |
     | Wi-Fi
     v
 MQTT Client
     |
     v
 MQTT Broker
     |
     v
 Cloud Platform

Example telemetry topic:

fleet/device001/telemetry

Example data:

{
  "device_id": "device001",
  "temperature": 5.8,
  "humidity": 62,
  "status": "NORMAL"
}

Network failure shall not prevent local monitoring and alerting.

17. Security Considerations

Future production firmware should consider:

Secure configuration
Device identity
Authentication
TLS communication
Secure firmware update
Firmware integrity
Access control
Protection of stored credentials

Security functionality shall be designed without compromising the
availability of local safety-related monitoring.

18. Firmware Modularity

The firmware shall be organized into independent modules.

Proposed structure:

firmware/
|
+-- include/
|   +-- config.h
|   +-- sensor_manager.h
|   +-- condition_classifier.h
|   +-- alert_manager.h
|   +-- event_logger.h
|   +-- configuration_manager.h
|
+-- src/
    +-- main.cpp
    +-- sensor_manager.cpp
    +-- condition_classifier.cpp
    +-- alert_manager.cpp
    +-- event_logger.cpp
    +-- configuration_manager.cpp

This modular structure improves:

Maintainability
Testability
Reusability
Debugging
Scalability
19. Firmware Design Principles

The firmware follows these principles:

Modular design
Separation of responsibilities
Input validation
Configurable thresholds
Hysteresis-based state stability
Local-first operation
Fault-aware behaviour
Persistent configuration
Event logging
Future network readiness
Security-aware architecture
Testability
20. Firmware Summary

The FleetGuard firmware uses a modular architecture centered around the
ESP32.

Sensor data is acquired by the Sensor Manager, validated, and passed to
the Condition Classifier.

The Condition Classifier determines NORMAL, WARNING, or CRITICAL state
using configurable thresholds and hysteresis.

The Alert Manager provides local OLED, LED, and buzzer indications.

The Event Logger maintains important system events, while the
Configuration Manager manages persistent parameters.

The architecture keeps core monitoring functions independent of future
network connectivity and provides a clear path toward Wi-Fi, MQTT,
security enhancements, and scalable fleet monitoring.

The architecture keeps core monitoring functions independent of future
network connectivity and provides a clear path toward Wi-Fi, MQTT,
security enhancements, and scalable fleet monitoring.
