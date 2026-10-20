# Mantis-Blades

Arduino-based wrist-mounted blade prop controller with printable parts, project documentation, and a maintainable control sketch.

## Project Highlights

This repo includes:

- Named configuration constants instead of scattered literals
- A `TiltServoController` class to isolate setup, sensor reads, filtering, and servo updates
- Non-blocking sample timing using `millis()` instead of a fixed `delay()` loop
- Clearer serial diagnostics and error handling
- Comments that explain the control flow and tuning points
- A cleaner repository layout for code and printable parts

## Repository Layout

```text
code/
  mantis-controller.ino
docs/
  assembly.md
  bom.md
  calibration.md
  safety.md
  testing.md
pieces/
  *.stl
assets/
  demo/
  diagrams/
```

## Hardware Overview

The current sketch assumes:

- `MPU6050` accelerometer/gyro
- 2 servo motors
- An Arduino-compatible board with `Wire` and `Servo` support

Pin assignments in the sketch:

- Primary servo: pin `9`
- Secondary servo: pin `10`

## Software Dependencies

Install these Arduino libraries before uploading:

- `Servo`
- `Wire`
- `MPU6050`

The exact `MPU6050` library varies by Arduino environment. Use the one that exposes:

- `initialize()`
- `testConnection()`
- `getAcceleration(...)`

## Build And Upload

1. Open [code/mantis-controller.ino](C:/Users/zhim/Repos/Mantis-Blades/code/mantis-controller.ino) in the Arduino IDE.
2. Install the required libraries if your environment does not already have them.
3. Select the target board and serial port.
4. Upload the sketch.
5. Open Serial Monitor at `9600` baud to verify sensor initialization and observe angle updates.

## Tuning

The main behavior controls live in the `Config` namespace:

- `FILTER_WEIGHT`: higher values smooth the motion more aggressively
- `TILT_GAIN`: increases or decreases servo response to wrist tilt
- `MOVEMENT_DEADBAND`: suppresses small updates that cause jitter
- `PRIMARY_MIN_ANGLE` / `PRIMARY_MAX_ANGLE`: output range for the main servo
- `SAMPLE_INTERVAL_MS`: update rate for the control loop

## Recommended Next Improvements

The next useful upgrades are:

- Fill in the placeholder docs in `docs/`
- Replace diagram placeholders with real wiring and pinout images
- Add measured calibration values for your sensor orientation
- Record a short demo and attach test evidence in `assets/demo/`
- Add exact sourcing details and print settings once hardware is finalized

## Documentation Placeholders

This repo now includes starter documents for the missing project material:

- [Bill of materials](C:/Users/zhim/Repos/Mantis-Blades/docs/bom.md)
- [Assembly guide](C:/Users/zhim/Repos/Mantis-Blades/docs/assembly.md)
- [Calibration guide](C:/Users/zhim/Repos/Mantis-Blades/docs/calibration.md)
- [Safety notes](C:/Users/zhim/Repos/Mantis-Blades/docs/safety.md)
- [Testing and demo evidence](C:/Users/zhim/Repos/Mantis-Blades/docs/testing.md)
