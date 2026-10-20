# Calibration Guide

This is a placeholder calibration workflow for the tilt-controlled sketch.

## Goal

Tune sensor orientation and servo output so the blades move predictably without jitter, binding, or overtravel.

## Initial Checks

- Open Serial Monitor at `9600` baud
- Confirm `MPU6050 initialized successfully.`
- Confirm both servos move to their starting angles on boot

## Tuning Parameters

These values live in [code/mantis-controller.ino](C:/Users/zhim/Repos/Mantis-Blades/code/mantis-controller.ino):

- `FILTER_WEIGHT`
- `TILT_GAIN`
- `MOVEMENT_DEADBAND`
- `PRIMARY_MIN_ANGLE`
- `PRIMARY_MAX_ANGLE`
- `SAFE_MIN_ANGLE`
- `SAFE_MAX_ANGLE`

## Suggested Procedure

1. Mount the sensor in its intended final orientation.
2. Upload the sketch with conservative angle limits.
3. Observe serial output while tilting the assembly slowly.
4. Adjust `TILT_GAIN` until motion range feels appropriate.
5. Increase `FILTER_WEIGHT` if movement is too noisy.
6. Increase `MOVEMENT_DEADBAND` if servos chatter at rest.
7. Tighten angle limits if the mechanism approaches a hard stop.

## Calibration Record

| Parameter | Trial Value | Final Value | Notes |
| --- | --- | --- | --- |
| `FILTER_WEIGHT` | TODO | TODO | |
| `TILT_GAIN` | TODO | TODO | |
| `MOVEMENT_DEADBAND` | TODO | TODO | |
| `PRIMARY_MIN_ANGLE` | TODO | TODO | |
| `PRIMARY_MAX_ANGLE` | TODO | TODO | |

## Open Items

- Add expected neutral orientation photos
- Record final tuning values from hardware tests
- Add failure modes and corrective actions
