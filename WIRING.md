# Wiring Guide

This wiring guide matches `src/obstacle_avoidance_robot.ino`. It assumes an Arduino Uno, HC-SR04 sensor, L298N motor-driver module, active buzzer, and two motor groups (left and right).

## Pin map

| Device pin | Connect to |
|---|---|
| HC-SR04 VCC | Arduino 5V |
| HC-SR04 GND | Common GND |
| HC-SR04 TRIG | Arduino D12 |
| HC-SR04 ECHO | Arduino D11 |
| Active buzzer signal (+) | Arduino D2 |
| Active buzzer (-) | GND |
| L298N ENA | Arduino D5 (PWM) |
| L298N IN1 | Arduino D7 |
| L298N IN2 | Arduino D8 |
| L298N ENB | Arduino D6 (PWM) |
| L298N IN3 | Arduino D9 |
| L298N IN4 | Arduino D10 |
| L298N OUT1 / OUT2 | Left motor group |
| L298N OUT3 / OUT4 | Right motor group |
| Battery negative | L298N GND |
| Arduino GND | L298N GND (common ground) |

## Important wiring notes

1. **The pin table and firmware must match.** If your assembled robot uses other pins, change the constants at the top of the `.ino` file or rewire the hardware.
2. For PWM speed control, connect Arduino D5 to ENA and D6 to ENB. On common L298N modules, remove the ENA/ENB jumper caps when using external PWM signals.
3. Connect both motors on the left side to the left driver channel and both motors on the right side to the right channel only if the combined current is within the driver's safe rating. Two motors in parallel draw more current than one.
4. Keep Arduino GND, sensor GND, and motor-driver GND connected together so the control signals have a common reference.
5. **Do not connect the lithium-ion battery directly to the Arduino 5V pin.**
6. The L298N board's 5V regulator and 5V terminal vary by module and jumper configuration. Do not connect its 5V terminal to the Arduino 5V rail unless you have verified the exact board instructions and power arrangement. During initial code testing, powering the Arduino through USB and powering the motor driver separately is a straightforward approach; grounds must still be common.
7. A two-cell (2S) lithium-ion pack is about 7.4 V nominal and 8.4 V fully charged. This is not automatically safe for 3–6 V motors. Use a suitable regulated motor supply or otherwise verify the voltage at the motors and their rated operating limits. The L298N is a motor driver, not a precision voltage regulator.
8. Confirm that the battery protection system, wiring, connectors, and motor driver can safely handle the combined motor current, including startup/stall current.
9. If a motor group moves backward when the firmware commands forward, power off and reverse that group's motor leads, or reverse its direction logic in code.
10. If using a buzzer that draws more current than an Arduino pin can safely supply, drive it through a suitable transistor circuit rather than directly from the pin.

## First-power checklist

- [ ] Raise the wheels so they cannot propel the car off the table.
- [ ] Check the battery polarity and all connections with power disconnected.
- [ ] Verify that no exposed battery terminal can touch the chassis or another terminal.
- [ ] Upload the sketch before enabling motor power.
- [ ] Test left/right motor directions at low risk.
- [ ] Confirm that the ultrasonic distance is printed in the Serial Monitor at 9600 baud.
- [ ] Test the obstacle response in an open area.
