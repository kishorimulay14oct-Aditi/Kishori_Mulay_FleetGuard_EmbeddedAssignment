# Technology Selection and Engineering Justification

## 1. Purpose

This document describes the major technology selections for the
FleetGuard Intelligent Refrigerated Container Monitoring System.

Each selection is based on functional requirements, reliability,
maintainability, cost, power consumption, development effort, and
future scalability.

The objective is to select technologies suitable for a proof-of-concept
while keeping the architecture suitable for future production evolution.

---

# 2. Technology Selection Summary

| Category | Selected Technology | Purpose |
|---|---|---|
| Main Controller | ESP32 | Edge processing and system control |
| Environmental Sensor | Digital Temperature/Humidity Sensor | Environmental monitoring |
| Local Display | OLED | Local status and measurement display |
| Visual Alert | RGB/Status LED | Immediate condition indication |
| Audible Alert | Buzzer | Local warning/critical notification |
| Local Communication | I2C | Sensor and display communication |
| Future Connectivity | Wi-Fi + MQTT | Cloud/fleet connectivity |
| Event Storage | Local non-volatile storage | Event history and configuration |
| Firmware Language | C/C++ | Embedded firmware development |
| Development Platform | PlatformIO/Arduino-compatible ESP32 environment | Firmware development and testing |

---

# 3. Main Controller Selection

## Selected: ESP32

The ESP32 is selected as the primary edge controller.

### Reasons for Selection

The ESP32 provides:

- Sufficient processing capability for local monitoring
- Integrated Wi-Fi
- Multiple GPIO interfaces
- I2C support
- SPI support
- UART support
- Low-power operating modes
- Large embedded software ecosystem
- Low-cost development hardware
- Good availability for prototyping
- Support for future connectivity requirements

The integrated Wi-Fi capability also reduces the need for a separate
communication module in the prototype.

---

## Alternatives Considered

### Arduino Uno

Advantages:

- Simple development environment
- Low cost
- Large educational ecosystem

Limitations:

- Limited processing and memory resources
- No integrated Wi-Fi
- Less suitable for future cloud connectivity
- Limited scalability for an IoT-oriented architecture

Therefore, Arduino Uno was not selected.

---

### Raspberry Pi

Advantages:

- High processing capability
- Linux operating system
- Large software ecosystem
- Easy cloud integration

Limitations:

- Higher power consumption
- Higher cost
- More complex than required for basic sensor monitoring
- Less appropriate for a small dedicated embedded monitoring node

Therefore, Raspberry Pi is not selected as the primary controller.

---

## Decision

ESP32 provides the best balance between:

- Cost
- Processing capability
- Connectivity
- Peripheral support
- Power efficiency
- Development simplicity
- Future scalability

Therefore, ESP32 is selected for the FleetGuard prototype.

---

# 4. Environmental Sensor Selection

## Selected: Digital Temperature and Humidity Sensor

A digital temperature/humidity sensor is selected for the prototype.

The sensor provides:

- Temperature measurement
- Relative humidity measurement
- Digital communication
- Simple integration with ESP32

A digital sensor reduces the need for external analog signal
conditioning and simplifies firmware integration.

---

## Sensor Selection Criteria

The final sensor shall be evaluated based on:

- Temperature measurement range
- Humidity measurement range
- Temperature accuracy
- Humidity accuracy
- Sampling rate
- Interface type
- Operating voltage
- Long-term availability
- Environmental suitability
- Cost

The exact production sensor shall be selected after considering the
required environmental range and accuracy of the target application.

---

## Engineering Consideration

For a refrigerated-container application, sensor accuracy is important
because small measurement errors can result in incorrect condition
classification.

Therefore, the prototype sensor should demonstrate functionality,
while production deployment requires a qualified industrial-grade sensor.

---

# 5. Local Display Selection

## Selected: OLED Display

An OLED display is selected for local status indication.

The display can show:

- Temperature
- Relative humidity
- Current system state
- Sensor status
- Warning messages
- Critical messages

### Reasons for Selection

- Compact size
- Low power consumption
- Good readability
- Simple digital interface
- Suitable for embedded applications
- Small number of required connections

---

## Alternative: Character LCD

A character LCD could provide basic text output.

However, an OLED provides better flexibility for displaying multiple
parameters and status information in a compact form.

Therefore, OLED is preferred.

---

# 6. Local Visual Alert Selection

## Selected: RGB/Status LED

An RGB or multi-color status LED is used to provide immediate visual
feedback.

The proposed indication is:

| Condition | LED Indication |
|---|---|
| Normal | Green |
| Warning | Yellow |
| Critical | Red |
| Sensor Failure | Fault indication |

The LED allows an operator to identify the system state without
navigating a display.

---

# 7. Audible Alert Selection

## Selected: Buzzer

A buzzer is used for audible notification.

### Warning

A short periodic beep can indicate a warning condition.

### Critical

A repeated or continuous alert can indicate a critical condition.

### Sensor Failure

A distinct fault pattern can be used to identify sensor problems.

The buzzer provides immediate attention when the operator is not
looking directly at the device.

---

# 8. Communication Interface Selection

## Selected: I2C for Local Peripherals

I2C is selected for communication between the ESP32 and suitable local
peripherals such as:

- Temperature/humidity sensor
- OLED display

### Advantages

- Uses only two main signal lines
- Supports multiple devices on the same bus
- Requires fewer GPIO pins
- Simple wiring
- Widely supported by embedded sensors and displays

### I2C Signals

```text
ESP32
  |
  +---- SDA ---- Sensor
  |
  +---- SDA ---- OLED
  |
  +---- SCL ---- Sensor
  |
  +---- SCL ---- OLED

Pull-up resistors shall be provided as required by the selected
hardware.

9. Other Communication Interfaces
SPI

SPI may be considered for:

High-speed displays
External memory
Storage devices

However, it is not required for the basic environmental monitoring
prototype.

UART

UART may be used for:

Debugging
Serial logging
External modules
Future cellular communication

UART is therefore retained as a useful development and expansion
interface.

10. Future Connectivity Selection
Selected: Wi-Fi + MQTT

Wi-Fi is available directly on the ESP32 and can provide network
connectivity for the prototype.

MQTT is selected as a future application-layer communication protocol.

Why MQTT?

MQTT is suitable for IoT applications because it provides:

Publish/subscribe communication
Lightweight messaging
Low bandwidth requirements
Device-to-cloud communication
Support for large numbers of devices
Topic-based data organization

Example:

FleetGuard Device
       |
       | MQTT Publish
       v
fleet/device001/telemetry
       |
       v
   MQTT Broker
       |
       +-------- Cloud Platform

The initial prototype focuses on local monitoring. Cloud deployment is
therefore considered future scope.

11. Local Event Storage
Selected: Non-Volatile Local Storage

The device shall maintain important events locally.

Potential storage options include:

ESP32 flash
NVS
External EEPROM
External flash
SD card for larger logging requirements

For the initial prototype, internal non-volatile storage can be used
for configuration and a limited event history.

For a production design with extensive historical data, external
storage or reliable cloud synchronization may be considered.

12. Firmware Language Selection
Selected: C/C++

C/C++ is selected for firmware development.

Reasons
Widely used in embedded systems
Efficient execution
Direct hardware access
Good ESP32 support
Large ecosystem of libraries
Suitable for modular firmware architecture
Supports real-time embedded programming

The firmware will be structured into independent modules rather than
placing all functionality inside a single source file.

13. Firmware Development Environment

The prototype can be developed using an ESP32-compatible development
environment such as:

PlatformIO
Arduino framework
ESP-IDF

For the initial proof-of-concept, an Arduino-compatible ESP32
environment provides a simple development path.

For production development, ESP-IDF can be considered when deeper
control over RTOS functionality, networking, security, and system
resources is required.

14. Cloud Architecture

The initial prototype does not require a complete cloud platform.

However, the architecture is designed to support:

+-------------+
| FleetGuard  |
| Edge Device |
+------+------+
       |
       | Wi-Fi
       v
+-------------+
| MQTT Broker |
+------+------+
       |
       v
+-------------+
| Cloud / IoT |
| Platform    |
+------+------+
       |
       v
+-------------+
| Fleet       |
| Dashboard   |
+-------------+

Potential cloud functions include:

Device registration
Telemetry storage
Alert monitoring
Historical data analysis
Fleet-level dashboards
Remote configuration
Device health monitoring
15. Security Technology Considerations

Security shall be considered from the beginning of the architecture.

The production system should consider:

Unique device identity
Secure device provisioning
Authenticated communication
TLS encryption
Secure configuration
Firmware integrity
Secure firmware updates
Access control

MQTT communication should use authenticated and encrypted connections
when deployed in production.

16. Power Considerations

The prototype shall use a regulated power supply suitable for the ESP32
and connected peripherals.

The production system may require additional power-management
considerations because refrigerated containers may operate continuously
and may experience power interruptions.

Potential improvements include:

Low-power operating modes
Sensor duty cycling
Efficient communication scheduling
Power-failure detection
Backup power
Brownout handling
17. Engineering Trade-Off Summary
Decision	Benefit	Trade-Off
ESP32	Wi-Fi + good processing + low cost	Not intended as a certified industrial controller
Digital sensor	Simple integration	Accuracy depends on selected sensor
OLED	Compact and flexible display	Smaller than industrial HMI
RGB LED	Simple status indication	Limited information
Buzzer	Immediate alert	Can be noisy in some environments
I2C	Simple multi-device bus	Limited distance and speed compared with some alternatives
Wi-Fi	Integrated and inexpensive	Requires network availability
MQTT	Lightweight IoT protocol	Requires broker/infrastructure
Local storage	Works without cloud	Limited capacity
C/C++	Efficient embedded execution	Requires careful memory/resource management
18. Final Technology Decision

The selected technology stack provides a practical balance between
prototype simplicity and future production scalability.

The proposed stack is:

ESP32
  |
  +---- Temperature/Humidity Sensor
  |
  +---- OLED Display
  |
  +---- RGB/Status LED
  |
  +---- Buzzer
  |
  +---- Local Non-Volatile Storage
  |
  +---- Wi-Fi
           |
           v
        MQTT
           |
           v
      Future Cloud

The architecture keeps the core monitoring and alerting functions local
so that temporary loss of network connectivity does not prevent the
device from detecting and reporting environmental problems.

19. Technology Selection Conclusion

ESP32 is selected as the edge controller because it provides sufficient
processing resources, integrated wireless connectivity, multiple
peripheral interfaces, and a mature development ecosystem at relatively
low cost.

Digital environmental sensors, OLED display, local LED and buzzer
provide the required monitoring and local alert functionality.

I2C provides a simple interface for local peripherals, while Wi-Fi and
MQTT provide a practical path toward future cloud connectivity.

The selected architecture therefore satisfies the proof-of-concept
requirements while leaving a clear path for future scalability,
security, and production evolution.


---

# 3.3 Commit it

At the bottom:

**Commit message:**

```text
Add technology selection and engineering justification
