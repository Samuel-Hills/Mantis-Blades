# Assembly Guide

This is a placeholder mechanical and electrical assembly guide.

## Before You Start

- Print and inspect all STL parts in `pieces/`
- Verify servo travel manually before installing blades
- Confirm the power source can handle both servos under load

## Mechanical Assembly

1. Print each STL part and remove supports.
2. Dry-fit the frame pieces before final fastening.
3. Mount the servos into the frame.
4. Attach the blade linkage and moving blade parts.
5. Install the wrist/arm brace and confirm fit.

## Electrical Assembly

1. Connect the `MPU6050` to the board over I2C.
2. Connect the primary servo signal to pin `9`.
3. Connect the secondary servo signal to pin `10`.
4. Connect servo power and ground to the external supply.
5. Tie controller ground and servo ground together.

## Photos To Add

- Exploded view of printed parts
- Servo mounting close-up
- Wiring layout
- Final assembled top and underside views

## Open Items

- Add exact screw sizes and insertion points
- Document linkage orientation
- Add cable routing notes
- Note left-hand vs right-hand wear orientation if relevant
