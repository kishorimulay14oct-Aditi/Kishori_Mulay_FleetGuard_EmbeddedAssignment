# FleetGuard Testing

## 1. Testing Overview

The FleetGuard system was tested to verify firmware functionality,
sensor monitoring, state transitions, event logging, configuration
handling, and system recovery.

## 2. Test Cases

| Test ID | Test Case | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| TC01 | System Startup | System initializes correctly | As expected | PASS |
| TC02 | Normal Sensor Operation | System remains in NORMAL state | As expected | PASS |
| TC03 | Sensor Failure | Sensor failure is detected and logged | As expected | PASS |
| TC04 | Sensor Recovery | Sensor recovery is detected and logged | As expected | PASS |
| TC05 | Normal → Warning | Warning state is triggered | As expected | PASS |
| TC06 | Warning → Critical | Critical state is triggered | As expected | PASS |
| TC07 | Critical → Warning | System returns to WARNING | As expected | PASS |
| TC08 | Warning → Normal | System returns to NORMAL | As expected | PASS |
| TC09 | Configuration Change | Configuration change is handled correctly | As expected | PASS |
| TC10 | Event Logging | Event count and latest event are stored | As expected | PASS |
| TC11 | Power Restart | System restarts and initializes correctly | As expected | PASS |
| TC12 | Firmware Build | Project compiles successfully in PlatformIO | Successful | PASS |

## 3. Event Logger Verification

The event logger was verified for the following events:

- SYSTEM_START
- SENSOR_FAILURE
- SENSOR_RECOVERY
- NORMAL_TO_WARNING
- WARNING_TO_CRITICAL
- CRITICAL_TO_WARNING
- WARNING_TO_NORMAL
- CONFIGURATION_CHANGE
- POWER_RESTART

The ESP32 NVS stores the event count and the latest event so that
the information remains available after a restart.

## 4. Build Verification

The firmware was successfully compiled using PlatformIO.

Result: **PASS**

## 5. Testing Summary

The implemented FleetGuard firmware passed the planned functional
and build-level verification checks. The repository can proceed to
final documentation, diagram verification, and GitHub review.
