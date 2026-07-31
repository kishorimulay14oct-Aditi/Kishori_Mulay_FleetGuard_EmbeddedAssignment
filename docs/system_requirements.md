# System Requirements

## 1. Purpose

This document defines the functional and non-functional requirements
for the FleetGuard Intelligent Refrigerated Container Monitoring System.

The purpose of the system is to continuously monitor environmental
conditions inside refrigerated transport containers and provide timely
detection and local indication of abnormal conditions.

The system is designed as an edge monitoring device with future
connectivity to a cloud platform.

---

## 2. Problem Definition

Refrigerated containers transporting temperature-sensitive products
require continuous monitoring of environmental conditions.

A failure to detect temperature or humidity excursions in time can
result in product damage, financial loss, regulatory issues, and
reduced customer confidence.

The proposed FleetGuard system addresses this problem by continuously
collecting environmental data, validating the measurements, classifying
the operating condition, generating alerts, and maintaining an event
history.

---

## 3. System Objectives

The system shall:

1. Continuously monitor temperature.
2. Monitor relative humidity.
3. Validate sensor readings.
4. Detect sensor failures and invalid readings.
5. Classify environmental conditions as Normal, Warning, or Critical.
6. Generate local visual and audible alerts.
7. Maintain a history of important system events.
8. Support configurable sampling intervals.
9. Support configurable threshold values.
10. Continue local monitoring during temporary communication failures.
11. Handle power restart conditions safely.
12. Provide a design suitable for future cloud connectivity.
13. Support scalable deployment across a large number of devices.
14. Consider security and firmware integrity for production deployment.

---

# 4. Functional Requirements

## FR-01: Temperature Monitoring

The system shall periodically measure the temperature inside the
refrigerated container.

### Input

Temperature sensor reading.

### Output

Temperature value in degrees Celsius.

### Requirement

The firmware shall periodically acquire and process the temperature
measurement according to the configured sampling interval.

---

## FR-02: Relative Humidity Monitoring

The system shall periodically measure relative humidity.

### Input

Humidity sensor reading.

### Output

Relative humidity percentage.

### Requirement

The firmware shall acquire and process humidity measurements together
with temperature measurements.

---

## FR-03: Sensor Data Validation

The system shall validate sensor readings before using them for
condition classification.

Validation shall identify:

- Sensor communication failure
- Missing readings
- Invalid numerical values
- Values outside the supported sensor range

Invalid readings shall not be treated as valid environmental
measurements.

---

## FR-04: Sensor Failure Detection

The system shall detect sensor failures.

When a sensor failure is detected, the system shall:

1. Generate a sensor failure event.
2. Activate an appropriate local indication.
3. Record the failure in the event history.
4. Attempt sensor recovery or reinitialization where applicable.
5. Continue operation without causing uncontrolled system failure.

---

### FR-05: Environmental Condition Classification

The system shall classify environmental conditions into three states:

#### Normal

All monitored parameters are within their configured safe operating
range.

#### Warning

One or more monitored parameters are approaching an unsafe limit.

#### Critical

One or more monitored parameters exceed the configured critical
threshold.

The classification thresholds shall be configurable.

---

### FR-05.1: Threshold Hysteresis

The system shall implement hysteresis around warning and critical
thresholds to prevent rapid switching between operating states when a
measured value fluctuates near a threshold.

Separate threshold values shall be used for entering and leaving an
operating state.

For the upper temperature warning threshold:

- Enter WARNING when temperature >= 8.0°C
- Return to NORMAL when temperature <= 7.5°C

For the upper temperature critical threshold:

- Enter CRITICAL when temperature >= 10.0°C
- Return from CRITICAL to WARNING when temperature <= 9.5°C

For the lower temperature limit:

- Enter WARNING when temperature <= 2.0°C
- Return to NORMAL when temperature >= 2.5°C
- Enter CRITICAL when temperature <= 0.0°C
- Return from CRITICAL to WARNING when temperature >= 0.5°C

The same hysteresis principle shall be applied to humidity thresholds
where required.

The threshold and hysteresis values shall be configurable.

The purpose of hysteresis is to prevent alarm/status chattering caused
by sensor noise and small environmental fluctuations around threshold
values.

---
# 6. Threshold Strategy

The proof-of-concept shall use configurable engineering demonstration
thresholds.

The system shall use hysteresis so that the operating state does not
rapidly change when a measured value fluctuates around a threshold.

## 6.1 Temperature Thresholds

The following values are proposed for demonstration:

| State Transition | Temperature |
|---|---:|
| NORMAL → WARNING | >= 8.0°C |
| WARNING → NORMAL | <= 7.5°C |
| WARNING → CRITICAL | >= 10.0°C |
| CRITICAL → WARNING | <= 9.5°C |
| NORMAL → WARNING (Low) | <= 2.0°C |
| WARNING → NORMAL (Low) | >= 2.5°C |
| WARNING → CRITICAL (Low) | <= 0.0°C |
| CRITICAL → WARNING (Low) | >= 0.5°C |

These are engineering demonstration values only.

## 6.2 Hysteresis Example

For the upper warning threshold:

```text
                 Temperature >= 8.0°C
NORMAL --------------------------------> WARNING
   ^                                       |
   |                                       |
   | Temperature <= 7.5°C                 |
   +---------------------------------------+

For the critical threshold:

                 Temperature >= 10.0°C
WARNING -------------------------------> CRITICAL
   ^                                       |
   |                                       |
   | Temperature <= 9.5°C                 |
   +---------------------------------------+

Example:

Temperature:
7.9 → 8.1 → 7.9 → 7.7 → 7.5°C

State:
NORMAL → WARNING → WARNING → WARNING → NORMAL

This prevents alarm/status chattering.

6.3 Humidity Thresholds

Humidity warning and critical thresholds shall also support hysteresis.

The exact values shall be selected based on the target application,
sensor accuracy, environmental requirements, and product requirements.

The threshold values shall be configurable rather than permanently
hard-coded into the classification algorithm.

6.4 Production Threshold Considerations

Actual production thresholds shall be determined from:

Product requirements
Customer requirements
Storage and transportation conditions
Sensor accuracy
Sensor noise characteristics
Regulatory requirements
Environmental qualification results

The proof-of-concept values must therefore be replaced with
application-specific limits before production deployment.


### Then find the table under:

```text
# 7. Requirement-to-Implementation Mapping

| Threshold hysteresis | Condition Classifier + Configuration Manager |

## FR-06: Local Alert Generation

The system shall provide local indication of abnormal conditions.

The prototype shall use:

- Status LED
- Buzzer
- OLED display

### Normal

- Green status indication
- Buzzer OFF
- Display indicates NORMAL

### Warning

- Yellow status indication
- Short audible alert
- Display indicates WARNING

### Critical

- Red status indication
- Repeated audible alert
- Display indicates CRITICAL

---

## FR-07: Event Logging

The system shall maintain a history of important events.

The event history shall include events such as:

- System boot
- Sensor initialization
- Sensor failure
- Sensor recovery
- Warning condition
- Critical condition
- Configuration change
- Power restart

Each event should include, where supported:

- Event type
- Timestamp or system time reference
- Relevant measured value
- Event severity

---

## FR-08: Configuration Management

The system shall support configurable operating parameters.

The configuration shall include:

- Sensor sampling interval
- Temperature warning threshold
- Temperature critical threshold
- Humidity warning threshold
- Humidity critical threshold
- Alert enable/disable settings

Configuration values shall be separated from the core monitoring logic
to improve maintainability and future customization.

---

## FR-09: Power Restart Handling

The system shall recover safely after a power interruption or restart.

After restart, the system shall:

1. Initialize the hardware.
2. Load the configured operating parameters.
3. Initialize the sensors.
4. Validate sensor availability.
5. Generate a boot/restart event.
6. Resume normal monitoring.

---

## FR-10: Communication Failure Handling

Future cloud communication shall not prevent local environmental
monitoring.

If communication with a cloud service becomes unavailable:

- Local monitoring shall continue.
- Local alerts shall continue.
- Events shall continue to be generated.
- Communication recovery shall be attempted periodically.

---

## FR-11: Future Connectivity

The architecture shall support future connectivity options including:

- Wi-Fi
- MQTT
- Cellular
- Ethernet
- LoRaWAN

The initial proof-of-concept shall focus on local monitoring and alerting.

---

# 5. Non-Functional Requirements

## NFR-01: Reliability

The system shall continue operating safely during expected sensor,
communication, and power-related faults.

A single sensor or communication failure should not cause uncontrolled
failure of the complete monitoring application.

---

## NFR-02: Maintainability

The firmware shall use modular components.

The major modules shall include:

- Sensor Manager
- Condition Classifier
- Alert Manager
- Event Logger
- Configuration Manager
- Main Application

This modular structure shall allow individual modules to be modified
or extended without requiring major changes to the complete application.

---

## NFR-03: Scalability

The architecture shall support deployment from a small number of
devices to a large fleet.

The design shall support:

- Approximately 10 devices for small deployments
- Approximately 100 devices for medium deployments
- 10,000 or more devices for large-scale deployments

Each device shall have a unique device identity for future fleet
management.

---

## NFR-04: Performance

The system shall process sensor measurements within the configured
sampling interval.

Local condition classification and alert generation shall occur
without unnecessary delays.

The system shall prioritize local monitoring and alert generation over
non-critical background communication tasks.

---

## NFR-05: Security

The production design shall consider:

- Unique device identity
- Protection against unauthorized configuration changes
- Firmware integrity
- Secure firmware update mechanisms
- Secure communication
- TLS for future cloud connectivity

Full production-grade security implementation is outside the scope of
the initial proof-of-concept.

---

## NFR-06: Usability

The local interface shall provide a simple indication of the current
environmental condition.

The operator should be able to distinguish:

- Normal condition
- Warning condition
- Critical condition
- Sensor failure

without requiring access to a cloud dashboard.

---

# 6. Threshold Strategy

The proof-of-concept shall use configurable engineering demonstration
thresholds.

For example, temperature classification may initially be represented
as:

| Condition | Temperature |
|---|---|
| Normal | 2°C to 8°C |
| Warning | 0°C to below 2°C OR above 8°C to 10°C |
| Critical | Below 0°C OR above 10°C |

These values are demonstration values only.

Actual production thresholds shall be determined from:

- Product requirements
- Customer requirements
- Storage and transportation conditions
- Sensor accuracy
- Regulatory requirements
- Environmental qualification results

The firmware shall therefore avoid hard-coding application-specific
limits into the monitoring algorithm.

---

# 7. Requirement-to-Implementation Mapping

| Requirement | Planned Implementation |
|---|---|
| Temperature monitoring | Temperature/Humidity Sensor + Sensor Manager |
| Humidity monitoring | Temperature/Humidity Sensor + Sensor Manager |
| Data validation | Sensor Manager |
| Sensor failure detection | Sensor Manager + Event Logger |
| Normal classification | Condition Classifier |
| Warning classification | Condition Classifier |
| Critical classification | Condition Classifier |
| Local indication | RGB/Status LED |
| Audible alert | Buzzer |
| Local display | OLED |
| Event history | Event Logger |
| Sampling configuration | Configuration Manager |
| Threshold configuration | Configuration Manager |
| Power restart handling | Main Application |
| Communication failure handling | Connectivity Manager / future module |
| Security | Device identity + secure architecture |
| Scalability | Unique device ID + future cloud architecture |

---

# 8. Assumptions

The following assumptions are made for the proof-of-concept:

1. Temperature and relative humidity are the primary monitored
   environmental parameters.
2. The ESP32 is used as the edge controller.
3. The selected sensor provides digital temperature and humidity data.
4. Threshold values are configurable.
5. Local monitoring shall continue even when cloud connectivity is
   unavailable.
6. The prototype is intended to demonstrate engineering feasibility
   rather than production certification.
7. Production hardware will require application-specific component
   qualification.

---

# 9. Constraints

The proof-of-concept has the following constraints:

- Limited prototype hardware availability
- Limited development time
- No requirement for a complete production cloud platform
- No requirement for production certification
- No requirement for full industrial enclosure qualification
- Security implementation is limited to architectural considerations
  where appropriate

---

# 10. Traceability Approach

The project will maintain traceability between:

Business Requirement
        ↓
System Requirement
        ↓
Architecture
        ↓
Firmware Implementation
        ↓
Test Case
        ↓
Test Result

This approach ensures that the prototype implementation can be
evaluated against the original system requirements.
