# AGENTS.md - Sim Handbrake

## Project Overview

Simulador de freno de mano USB para sim racing. Convierte la lectura analógica de un potenciómetro en un eje de joystick USB reconocido automáticamente por simuladores de carreras.

**Target Hardware**: Arduino Leonardo (ATmega32U4)  
**USB Product**: "Handbrake"  
**Framework**: Arduino  
**Build System**: PlatformIO

## Project Structure

```
sim-handbreak/
├── src/
│   └── main.cpp                    # Entry point - configuration only
├── lib/
│   └── handbrake/                  # Core library
│       ├── include/
│       │   └── handbrake/
│       │       ├── AnalogSensor.h
│       │       ├── MovingAverageFilter.h
│       │       ├── Calibrator.h
│       │       ├── JoystickSender.h
│       │       └── HandbrakeController.h
│       └── src/
│           ├── AnalogSensor.cpp
│           ├── MovingAverageFilter.cpp
│           ├── Calibrator.cpp
│           ├── JoystickSender.cpp
│           └── HandbrakeController.cpp
├── platformio.ini
└── README.md
```

## Build Commands

```bash
# Compile for Arduino Leonardo
pio run

# Upload to device
pio run -t upload

# Clean build
pio run -t clean

# Monitor serial output
pio device monitor

# Compile and upload
pio run -t upload && pio device monitor
```

## Architecture

### SOLID Principles Applied

| Class | Responsibility | SOLID Principle |
|-------|---------------|-----------------|
| `AnalogSensor` | Read analog pin | SRP |
| `MovingAverageFilter` | Signal filtering | SRP |
| `Calibrator` | Map/calibrate values | SRP, OCP |
| `JoystickSender` | USB HID output | SRP |
| `HandbrakeController` | Orchestrate pipeline | SRP, DIP |

### Data Flow

```
Potentiometer → AnalogSensor → MovingAverageFilter → Calibrator → JoystickSender → USB HID
      │              │                │                   │              │
      ▼              ▼                ▼                   ▼              ▼
   Analog Read   Raw Value      Filtered Value      Mapped 0-1023    USB Report
```

### Class Dependencies

```
HandbrakeController (Facade)
├── AnalogSensor*        (injected)
├── MovingAverageFilter* (injected)
├── Calibrator*          (injected)
└── JoystickSender*      (injected)
```

## Configuration Parameters

### Hardware (main.cpp)

| Parameter | Value | Description |
|-----------|-------|-------------|
| `POT_PIN` | A3 | Analog input pin |
| `FILTER_SAMPLES` | 8 | Moving average window size |
| `ADC_REPOSO` | 945 | ADC value at rest position |
| `ADC_A_FONDO` | 735 | ADC value at full pull |

### Calibration

The `CalibrationConfig` struct allows custom input/output ranges:

```cpp
CalibrationConfig config = {
    .inputMin = 945,    // ADC rest value
    .inputMax = 735,    // ADC full pull value
    .outputMin = 0,     // Joystick min
    .outputMax = 1023   // Joystick max
};
```

### Timing

- Update rate: 100Hz (10ms delay)
- USB reports sent manually (begin(false))

## Code Conventions

- **Language**: C++11
- **Naming**:
  - Classes: `PascalCase`
  - Methods: `camelCase`
  - Member variables: `_camelCase` (underscore prefix)
  - Constants: `UPPER_SNAKE_CASE`
- **Headers**: `#pragma once`
- **Memory**: Prefer stack allocation, use `new`/`delete` only when necessary
- **Includes**: Use `handbrake/` prefix for library headers

## Dependencies

### External Libraries

| Library | Version | Purpose |
|---------|---------|---------|
| `mheironimus/Joystick` | ^2.1.1 | USB HID Joystick emulation |

### Platform

| Component | Version |
|-----------|---------|
| Platform | atmelavr 4.2.0 |
| Board | leonardo |
| Framework | arduino 5.1.0 |
| Toolchain | 7.3.0 |

## Hardware Notes

- **Board**: Arduino Leonardo (ATmega32U4 with native USB)
- **USB**: Native USB HID, no drivers needed on most OS
- **ADC**: 10-bit (0-1023)
- **Calibration**: Measure actual pot values with multimeter, update `ADC_REPOSO` and `ADC_A_FONDO`

## Troubleshooting

### Build Fails

```bash
# Clean and rebuild
pio run -t clean && pio run

# Check library dependencies
pio pkg list
```

### Upload Fails

```bash
# Reset Arduino Leonardo (double-tap reset button)
# Then upload quickly
pio run -t upload
```

### Joystick Not Detected

- Verify USB cable supports data (not charge-only)
- Check `board_build.usb_product` in `platformio.ini`
- Test with joy.cpl (Windows) or jstest (Linux)

## Future Improvements

- [ ] Add deadzone configuration
- [ ] Support multiple calibration presets
- [ ] Add EEPROM storage for calibration
- [ ] Implement auto-calibration routine
- [ ] Add LED status indicator
- [ ] Support multiple axis mappings
