# Testing and Troubleshooting

## Upload and first test

1. Install the Arduino IDE.
2. Open `src/obstacle_avoidance_robot.ino`.
3. Select **Arduino Uno** under Tools → Board (if you have an Uno-compatible controller).
4. Select the correct port under Tools → Port.
5. Connect the Arduino by USB and upload the sketch.
6. Open Serial Monitor at **9600 baud**.
7. Initially test with the wheels raised. Keep motor power disconnected while checking the sensor and code upload.
8. Once sensor readings look plausible, enable motor power and test on the floor at low speed in a clear area.

## Expected behaviour

The code uses these thresholds:

- More than 35 cm: forward at PWM 200, buzzer off.
- 20–35 cm: forward at PWM 130, pulsed buzzer.
- Less than 20 cm: buzzer on while the robot pivots left briefly.

These values are firmware settings, not guaranteed stopping distances. Tune them only after the robot is mechanically secure and behaving predictably.

## Troubleshooting

### Serial Monitor says `NO ECHO - stopping for safety`
- Check TRIG and ECHO pins against the wiring table.
- Confirm sensor VCC and GND.
- Aim the sensor toward a broad, flat target within its useful range.
- Confirm that the sensor is not loose and its signal wires are connected correctly.

### A motor side moves in the wrong direction
- Turn off power first.
- Reverse the two motor wires for that side, or invert its direction logic in the sketch.
- Re-test with wheels lifted.

### Motors do not move
- Check battery charge and polarity.
- Check common ground.
- Check ENA/ENB wiring and the enable jumpers on the driver.
- Verify that the motor driver is rated for the combined current drawn by the motors.

### Arduino restarts when motors start
- Motor current spikes or electrical noise may be pulling down the logic supply.
- Use a suitable motor supply, correct wiring, common ground, and appropriate decoupling.
- Do not power the motors from Arduino GPIO pins or the Arduino 5V pin.
- Check for overloaded motor driver or weak battery.

### Buzzer does not sound
- Confirm that it is an **active** buzzer and its polarity and signal pin are correct.
- A passive buzzer may require `tone()` rather than a steady HIGH signal.
