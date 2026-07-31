# FleetGuard System Architecture

## 1. Purpose

The FleetGuard Intelligent Refrigerated Container Monitoring System is
an edge-based environmental monitoring system designed for refrigerated
containers.

The system continuously monitors temperature and relative humidity,
validates sensor data, classifies the environmental condition, provides
local alerts, and records important events.

The ESP32 acts as the central edge controller.

The core monitoring and alerting functions are performed locally so that
the system can continue operating even when network connectivity is
unavailable.

---

## 2. High-Level Architecture

```text
                  +-----------------------------+
                  | Temperature & Humidity      |
                  |           Sensor            |
                  +-------------+---------------+
                                |
                                | Sensor Data
                                v
                  +-----------------------------+
                  |            ESP32             |
                  |       Edge Controller       |
                  +-------------+---------------+
                                |
          +---------------------+---------------------+
          |                     |                     |
          v                     v                     v
 +----------------+    +-------------------+   +--------------------+
 | Sensor Manager |    | Condition         |   | Configuration       |
 |                |    | Classifier        |   | Manager             |
 +----------------+    +---------+---------+   +--------------------+
                                 |
                                 v
                        +------------------+
                        |   Alert Manager  |
                        +--------+---------+
                                 |
                 +---------------+---------------+
                 |               |               |
                 v               v               v
          +------------+   +------------+   +------------+
          |    OLED    |   | RGB /      |   |  Buzzer    |
          |  Display   |   | Status LED |   |   Alert    |
          +------------+   +------------+   +------------+

                                 |
                                 v
                        +------------------+
                        |   Event Logger   |
                        +--------+---------+
                                 |
                                 v
                        +------------------+
                        |  Local Storage   |
                        +--------+---------+
                                 |
                                 | Future Wi-Fi
                                 v
                        +------------------+
                        |   MQTT Broker    |
                        +--------+---------+
                                 |
                                 v
                        +------------------+
                        | Future Cloud /   |
                        | Fleet Dashboard  |
                        +------------------+
3. Main Components
3.1 Temperature and Humidity Sensor

The environmental sensor provides temperature and relative humidity
measurements to the ESP32.

The sensor is periodically sampled according to the configured sampling
interval.

The sensor data is passed to the Sensor Manager for validation before
being used for condition classification.

3.2 ESP32 Edge Controller

The ESP32 is the central processing unit of the prototype.

Its responsibilities include:

Sensor acquisition
Sensor data validation
Condition classification
Hysteresis processing
Alert generation
Event logging
Configuration management
OLED display control
RGB/status LED control
Buzzer control
Future Wi-Fi communication

The ESP32 performs the critical monitoring functions locally.

3.3 Sensor Manager

The Sensor Manager is responsible for:

Initializing the environmental sensor
Reading temperature
Reading relative humidity
Checking sensor communication
Validating sensor readings
Detecting invalid sensor data
Detecting sensor failure
Reporting sensor status to the application

Invalid or unavailable sensor data shall not be treated as valid
environmental measurements.

3.4 Condition Classifier

The Condition Classifier determines the environmental condition.

The system supports three states:

NORMAL
WARNING
CRITICAL

The classification is based on configurable temperature and humidity
thresholds.

The Condition Classifier also implements hysteresis to prevent rapid
switching between states when sensor values fluctuate near a threshold.

4. Threshold Hysteresis

Hysteresis uses different threshold values for entering and leaving a
condition.

For example, consider the upper temperature warning threshold.

                 Temperature >= 8.0°C
NORMAL --------------------------------> WARNING
   ^                                       |
   |                                       |
   | Temperature <= 7.5°C                 |
   +---------------------------------------+

The system enters WARNING when:

Temperature >= 8.0°C

The system returns to NORMAL only when:

Temperature <= 7.5°C

Therefore, small temperature fluctuations around 8.0°C will not cause
rapid switching between NORMAL and WARNING.

Critical Threshold Hysteresis

For the critical temperature threshold:

                 Temperature >= 10.0°C
WARNING -------------------------------> CRITICAL
   ^                                       |
   |                                       |
   | Temperature <= 9.5°C                 |
   +---------------------------------------+

The system enters CRITICAL when:

Temperature >= 10.0°C

The system returns from CRITICAL to WARNING only when:

Temperature <= 9.5°C

The same principle shall be applied to lower temperature limits and
humidity limits where required.

The actual hysteresis values are engineering demonstration values and
shall be configurable for the target application.

5. Alert Manager

The Alert Manager provides local indication of the current system state.

It controls:

OLED display
RGB/status LED
Buzzer

The proposed indication is:

Condition	LED	Buzzer	OLED
Normal	Green	OFF	NORMAL
Warning	Yellow	Periodic beep	WARNING
Critical	Red	Repeated alert	CRITICAL
Sensor Failure	Fault indication	Fault pattern	SENSOR ERROR

Local alerts shall continue to operate even if network connectivity is
unavailable.

6. Event Logger

The Event Logger records important system events.

Examples include:

System boot
Sensor initialization
Sensor failure
Sensor recovery
Warning condition
Critical condition
Configuration change
Power restart
Communication status change

The event history provides information for troubleshooting and future
fleet monitoring.

7. Configuration Manager

The Configuration Manager maintains configurable parameters such as:

Sampling interval
Temperature warning threshold
Temperature critical threshold
Temperature hysteresis
Humidity warning threshold
Humidity critical threshold
Humidity hysteresis
Alert settings

Keeping configuration separate from monitoring logic improves
maintainability and allows application-specific limits to be changed
without redesigning the complete firmware architecture.

8. Local User Interface

The local user interface consists of:

OLED Display

The OLED can display:

Temperature
Relative humidity
Current condition
Sensor status
Warning messages
Critical messages
Fault information
RGB/Status LED

The LED provides quick visual indication:

GREEN  → NORMAL
YELLOW → WARNING
RED    → CRITICAL
FAULT  → SENSOR / SYSTEM ERROR
Buzzer

The buzzer provides audible notification for warning, critical, and
fault conditions.

9. Local Event Storage

The system shall maintain important events using local non-volatile
storage.

Possible storage options include:

ESP32 flash
NVS
External EEPROM
External flash
SD card

For the initial proof-of-concept, internal non-volatile storage may be
used for configuration and a limited event history.

Production systems requiring extensive historical storage may use
external storage or cloud synchronization.

10. Data Flow

The normal data flow is:

+-----------------------------+
| Temperature / Humidity       |
| Sensor                       |
+-------------+---------------+
              |
              v
+-----------------------------+
| Sensor Manager              |
+-------------+---------------+
              |
              v
+-----------------------------+
| Data Validation             |
+-------------+---------------+
              |
              v
+-----------------------------+
| Condition Classifier        |
+-------------+---------------+
              |
              v
+-----------------------------+
| Threshold Evaluation        |
+-------------+---------------+
              |
              v
+-----------------------------+
| Hysteresis Check            |
+-------------+---------------+
              |
              v
+-----------------------------+
| Current System State        |
+------+------+---------------+
       |      |
       |      +----------------------+
       |                             |
       v                             v
+-------------+              +---------------+
| Alert       |              | Event Logger  |
| Manager     |              |               |
+------+------+              +-------+-------+
       |                             |
       +---------+---------+         |
       |         |         |         |
       v         v         v         v
     OLED      LED      Buzzer   Local Storage
11. Fault Handling

The system shall detect sensor failures and invalid sensor data.

The fault flow is:

Sensor Reading
      |
      v
+------------------+
| Data Valid?      |
+--------+---------+
         |
      +--+--+
      |     |
     YES    NO
      |     |
      v     v
 Classification   Sensor Failure
      |                 |
      |          +------+------+
      |          |             |
      |          v             v
      |       Alert         Event Log
      |
      v
 Local Indication

A sensor failure shall not be interpreted as a normal environmental
condition.

The system shall provide a clear fault indication to the operator.

12. Future Connectivity

The ESP32 provides Wi-Fi capability for future connectivity.

MQTT is proposed as the future application-layer communication
protocol.

The future communication path is:

+------------------+
| FleetGuard ESP32 |
+--------+---------+
         |
         | Wi-Fi
         v
+------------------+
|   MQTT Broker    |
+--------+---------+
         |
         v
+------------------+
| Cloud / IoT      |
| Platform         |
+--------+---------+
         |
         v
+------------------+
| Fleet Dashboard  |
+------------------+

The initial prototype focuses on local monitoring.

Cloud deployment is therefore considered future scope.

13. Network Independence

The architecture separates local monitoring from network communication.

             LOCAL MONITORING
+----------------------------------------+
| Sensor                                 |
|   ↓                                    |
| ESP32                                   |
|   ↓                                    |
| Validation                              |
|   ↓                                    |
| Classification + Hysteresis             |
|   ↓                                    |
| LED / Buzzer / OLED                     |
|   ↓                                    |
| Event Logging                           |
+----------------------------------------+
                    |
                    | Future Connectivity
                    v
             NETWORK DOMAIN
+----------------------------------------+
| Wi-Fi → MQTT → Cloud → Dashboard      |
+----------------------------------------+

If Wi-Fi or MQTT communication becomes unavailable, the local
monitoring, classification, alerting, and event logging functions shall
continue to operate.

14. Security Considerations

Security shall be considered from the beginning of the architecture.

The production implementation should consider:

Unique device identity
Secure device provisioning
Authenticated communication
TLS encryption
Secure configuration
Firmware integrity verification
Secure firmware update mechanism
Access control

MQTT communication should use authenticated and encrypted connections
for production deployment.

15. Scalability

The architecture is designed so that multiple FleetGuard devices can
operate independently.

+-------------+       |
| Device 001  |-------|
+-------------+       |
                      |
+-------------+       |
| Device 002  |-------+----> MQTT / Cloud
+-------------+       |
                      |
+-------------+       |
| Device 003  |-------|
+-------------+       |
                      |
        ...           |
                      |
+-------------+       |
| Device N    |-------|
+-------------+

Each device shall have a unique device identifier.

This allows future fleet-level monitoring, device health monitoring,
historical analysis, and centralized alert management.

16. Architecture Design Principles

The architecture follows these principles:

Local-first monitoring
Modular firmware design
Separation of sensing and classification
Sensor data validation before classification
Hysteresis-based stable state transitions
Separation of configuration and application logic
Fault-aware operation
Network-independent local alerting
Security-aware design
Future connectivity readiness
Scalable device architecture
Maintainability
17. Architecture Summary

The FleetGuard architecture places the ESP32 at the center of the
edge-monitoring system.

Environmental data is acquired from the temperature and humidity sensor
and passed through validation before classification.

The Condition Classifier determines whether the environment is NORMAL,
WARNING, or CRITICAL.

Hysteresis is used to prevent rapid state changes caused by small
measurement fluctuations around threshold values.

The Alert Manager provides immediate local feedback through the OLED,
RGB/status LED, and buzzer.

Important events are recorded locally.

Wi-Fi and MQTT provide a future path to cloud connectivity while keeping
the core monitoring and alerting functions independent of the network.

This architecture provides a foundation for extending the prototype
toward a scalable, secure, and production-oriented fleet monitoring
solution.



