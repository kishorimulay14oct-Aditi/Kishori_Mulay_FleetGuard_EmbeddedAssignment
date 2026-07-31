# FleetGuard Hardware Design

## 1. Hardware Overview

The FleetGuard prototype is designed around an ESP32 microcontroller
with temperature and humidity sensing, local visual and audible
indication, display capability, optional door-status monitoring, and a
regulated power supply.

The hardware architecture is intended to support reliable local
monitoring while keeping the design simple enough for a proof-of-concept.

---

## 2. Hardware Components

The proposed prototype consists of:

| Component | Purpose |
|---|---|
| ESP32 | Main edge controller |
| Temperature/Humidity Sensor | Environmental monitoring |
| OLED Display | Local information display |
| RGB/Status LED | Visual condition indication |
| Buzzer | Audible alert |
| Door-Status Sensor | Optional door-open detection |
| Non-Volatile Storage | Configuration and event history |
| Power Supply | Provides regulated power |
| Connecting Interface | Interconnects sensors and peripherals |

---

## 3. ESP32 Microcontroller

### Selection

ESP32 is selected as the primary microcontroller for the prototype.

### Reasons for Selection

- Integrated Wi-Fi capability
- Sufficient processing capability for the application
- Multiple GPIO interfaces
- I2C support
- SPI support
- UART support
- Low-cost development platform
- Large development ecosystem
- Good support for embedded and IoT applications
- Suitable for future MQTT connectivity

### Role in FleetGuard

The ESP32 performs the following functions:

- Sensor acquisition
- Sensor data validation
- Environmental condition classification
- Hysteresis processing
- Alert control
- OLED control
- Event logging
- Configuration management
- Future Wi-Fi communication

---

## 4. Temperature and Humidity Sensor

A digital temperature and relative humidity sensor is used to monitor
the environmental conditions inside the refrigerated container.

### Requirements

The selected sensor should provide:

- Digital output
- Suitable temperature measurement range
- Suitable humidity measurement range
- Adequate accuracy
- Stable operation
- Simple microcontroller interface

### Role

The sensor periodically provides:

```text
Temperature
Relative Humidity

The readings are passed to the Sensor Manager for validation.

5. OLED Display

An OLED display is selected for local visualization.

Display Information

The display may show:

Temperature: 5.8 C
Humidity:    62 %
Status:      NORMAL

During abnormal conditions:

Temperature: 9.2 C
Humidity:    65 %
Status:      WARNING

For critical conditions:

Temperature: 10.5 C
Humidity:    67 %
Status:      CRITICAL
Advantages
Compact
Low power
Good visibility
Simple digital interface
Suitable for embedded prototypes
6. RGB / Status LED

An RGB LED or equivalent status LED provides quick visual feedback.

The proposed indication is:

System State	LED Indication
Normal	Green
Warning	Yellow
Critical	Red
Sensor/System Fault	Fault indication

The LED provides a simple indication without requiring the operator to
read the display.

7. Buzzer

A buzzer provides audible alerts.

Proposed Behaviour
Condition	Buzzer
Normal	OFF
Warning	Periodic beep
Critical	Repeated/continuous alert
Sensor Fault	Fault pattern

The exact alert pattern shall be configurable during firmware
implementation.

8. Optional Door-Status Sensor

An optional door-status sensor may be added to detect whether the
container door is open or closed.

Possible implementations include:

Magnetic reed switch
Hall-effect sensor
Digital door sensor

The door-status information can be used for:

Door-open event logging
Alert generation
Future security monitoring
Correlation with temperature changes

The door sensor is optional for the basic environmental monitoring
prototype.

9. Local Non-Volatile Storage

The system requires local storage for configuration and important
events.

Possible options include:

ESP32 flash
NVS
External EEPROM
External flash
SD card
Prototype Selection

Internal ESP32 non-volatile storage may be used initially for:

Configuration parameters
Threshold values
Sampling interval
Limited event history

For larger production-scale historical data, external storage or cloud
synchronization may be considered.

10. Communication Interfaces
I2C

I2C is suitable for local peripherals such as:

OLED display
Digital sensors
Other low-speed peripherals

Advantages include:

Two-wire interface
Multiple devices on the same bus
Simple wiring
Good support on ESP32
SPI

SPI may be considered for:

High-speed displays
External memory
Storage devices

SPI is not required for the basic environmental monitoring prototype.

UART

UART may be used for:

Debugging
Serial logging
External modules
Future cellular communication

UART is therefore retained as a useful development and expansion
interface.

Wi-Fi

Wi-Fi is provided by the ESP32 and is intended for future network
connectivity.

11. Power Supply

The prototype shall use a regulated power supply suitable for the ESP32
and connected peripherals.

The power subsystem shall provide:

Stable supply voltage
Adequate current capacity
Protection appropriate to the prototype
Reliable operation of connected peripherals

The production system may require additional power-management
considerations because refrigerated containers can operate continuously.

12. Power Failure and Restart Handling

The system shall consider power interruptions and restart behaviour.

After a restart, the ESP32 should:

Initialize the system.
Initialize connected peripherals.
Load stored configuration.
Initialize the environmental sensor.
Validate sensor operation.
Restore normal monitoring operation.
Record a system restart event.

The design should avoid losing critical configuration during a power
restart.

13. Hardware Block Diagram

The conceptual hardware architecture is:

                  +----------------------+
                  |      Power Supply    |
                  +----------+-----------+
                             |
                             v
                  +----------------------+
                  |        ESP32         |
                  |   Edge Controller    |
                  +----+----+----+-------+
                       |    |    |
              +--------+    |    +----------+
              |             |               |
              v             v               v
       +-------------+  +--------+    +------------+
       | Temperature |  | OLED   |    | RGB / LED  |
       | & Humidity  |  |Display |    |            |
       |   Sensor    |  +--------+    +------------+
       +-------------+
                       |
                       v
                  +----------+
                  |  Buzzer  |
                  +----------+

                  Optional:
                  +----------------+
                  | Door Sensor    |
                  +----------------+
14. Hardware Interface Summary
Peripheral	Interface	Connection to ESP32
Temperature/Humidity Sensor	Digital / I2C	GPIO / I2C
OLED	I2C	SDA + SCL
RGB LED	GPIO/PWM	GPIO
Buzzer	GPIO/PWM	GPIO
Door Sensor	GPIO	GPIO
External Storage	I2C/SPI	I2C/SPI
Debug Interface	UART	UART
Cloud Connectivity	Wi-Fi	Integrated ESP32 Wi-Fi

The exact GPIO assignments shall be finalized during firmware and
hardware integration.

15. Hardware Design Considerations

The following factors shall be considered during prototype
implementation:

Sensor placement
Sensor accuracy
Electrical noise
Power stability
GPIO availability
I2C address conflicts
Pull-up requirements
Grounding
Connector reliability
Environmental conditions
Cable length
Enclosure requirements

For production deployment, the hardware shall additionally be evaluated
for temperature range, humidity, vibration, electrical protection,
electromagnetic compatibility, and applicable industry requirements.

16. Hardware Design Trade-Offs
Component	Benefit	Trade-Off
ESP32	Wi-Fi, processing capability, low cost	Prototype-oriented controller
Digital Sensor	Simple integration	Accuracy depends on selected device
OLED	Compact and flexible	Limited display area
RGB LED	Simple status indication	Limited information
Buzzer	Immediate audible alert	May be unsuitable in noisy environments
I2C	Simple multi-device connection	Limited distance
Local Storage	Works without cloud	Limited capacity
Door Sensor	Adds useful event information	Additional hardware required
17. Engineering Considerations

The component selection in this document represents a proof-of-concept
architecture.

Final production component selection shall be based on:

Required operating temperature range
Required measurement accuracy
Long-term sensor stability
Power requirements
Environmental protection
Availability
Cost
Reliability
Product qualification requirements
Regulatory requirements

The proof-of-concept hardware shall therefore not be considered a
production-certified design without further engineering validation.

18. Hardware Summary

The proposed hardware provides all functions required for the initial
FleetGuard monitoring prototype.

The ESP32 provides the central processing and future Wi-Fi capability.

The environmental sensor provides temperature and humidity measurements.

The OLED, RGB/status LED, and buzzer provide local user feedback.

Optional door sensing and expanded local storage can be added as the
system evolves.

The hardware architecture therefore provides a practical foundation
for implementing the FleetGuard embedded monitoring system while
allowing future expansion toward a production-oriented design.
