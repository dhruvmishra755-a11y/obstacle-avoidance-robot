# Obstacle-Avoiding Robot: Failure Cases, Loopholes, and Fixes

> **Project:** Arduino-based 4WD obstacle-avoiding robot  
> **Typical setup:** Arduino-compatible controller, HC-SR04 ultrasonic sensor, servo-mounted sensor (if fitted), motor driver, DC motors, battery, and buzzer.  
> **Purpose:** Document situations in which the robot may fail to detect an obstacle or may choose an unsafe movement, along with practical ways to reduce those failures.

---

## 1. Why this document exists

An obstacle-avoiding robot does not understand its surroundings like a human. It makes decisions from sensor readings and the logic in its program. If an obstacle is outside the sensor's effective field of view, produces an unreliable echo, or is not checked before movement, the robot may collide with it.

This document records likely failure cases and test ideas. It is not a guarantee that the robot will be collision-proof. Test at low speed in a clear area, keep a hand near the power switch, and never test near stairs, roads, people, pets, or fragile objects.

## 2. Important limitations and failure cases

### Case 1 — The obstacle is not directly in front of the ultrasonic sensor

**What happens:**  
The HC-SR04 points in one direction and has a limited detection field. An obstacle far to the left or right may not return a useful echo. A robot can therefore hit an object that is beside its forward path, especially while turning.

**How to reduce the risk:**
- Mount the sensor facing forward and keep it firmly fixed.
- Keep the sensor's view clear of the chassis, wires, and decorations.
- If a servo is fitted, scan left and right before choosing a turn.
- Use side sensors or bump switches if side coverage is required.
- Test obstacles at several angles; do not assume the sensor sees a full 180° area.

**Test:** Place a box directly ahead, then move it gradually to the left and right. Record the angle and distance at which readings become unreliable.

### Case 2 — The obstacle is above or below the sensor's beam

**What happens:**  
The sensor can miss a low object, a high overhang, or an object whose surface does not reflect the sound back to the receiver. The robot may pass under or collide with an object that is not at the sensor's height.

**How to reduce the risk:**
- Mount the sensor at a height appropriate for the obstacles expected in the robot's environment.
- Aim it slightly downward only if low obstacles are the priority; verify that this does not reduce useful forward range.
- Add a second sensor at a different height if the robot must detect both low and high obstacles.
- Use a bumper switch for objects that can still be missed by ultrasonic sensing.

**Test:** Try a short box, a tall box, a thin upright object, and an overhanging object. Test each at the same distance.

### Case 3 — Soft, angled, narrow, or irregular surfaces give weak echoes

**What happens:**  
Cloth, foam, slanted surfaces, narrow poles, and irregular shapes may absorb or redirect sound. The sensor may return no echo or a distance that jumps around.

**How to reduce the risk:**
- Treat a missing or invalid echo as an unknown condition, not proof that the path is clear.
- Take multiple readings and use a simple median or other robust filter.
- Stop and recheck when readings are inconsistent.
- Consider a different sensing technology or a physical bumper for important use cases.

**Test:** Compare a flat cardboard box, cloth, a slanted book, and a narrow pole from the same distance.

### Case 4 — The obstacle is too close for a reliable measurement

**What happens:**  
Ultrasonic modules have a minimum usable distance. If the robot starts extremely close to a wall, or moves forward before the first valid measurement, the reading may be unreliable.

**How to reduce the risk:**
- Start with a clear space in front of the robot.
- Take a valid reading before driving forward.
- If the measurement is invalid or below the safe threshold, stop rather than continue forward.
- Choose a threshold that accounts for sensor limits and the robot's stopping distance.

**Test:** Start at several distances from a flat wall and note where the readings stop being reliable.

### Case 5 — The robot moves too fast to stop in time

**What happens:**  
The sensor detects an obstacle, but the robot's momentum and motor response carry it forward before it stops.

**How to reduce the risk:**
- Reduce motor speed during testing.
- Set the stopping threshold using measured stopping distance, not guesswork.
- Recheck distance frequently while moving.
- Include a margin for sensor error, processing delay, and motor-driver response.

**Test:** At each speed setting, measure how far the robot travels after a stop command. Set the threshold comfortably above that distance.

### Case 6 — The robot chooses a turn without checking that the route is clear

**What happens:**  
A basic program may turn right every time it sees an obstacle. If the right side is blocked, the robot can collide or become trapped.

**How to reduce the risk:**
- If a servo is fitted, stop first and measure left and right.
- Compare valid readings and choose the side with more clearance.
- If both sides are blocked, stop, reverse a short controlled distance if safe, and scan again.
- Add a turn timeout and a maximum number of repeated recovery attempts.

**Test:** Put obstacles ahead and on the right, then ahead and on the left, and finally on both sides. Confirm the robot does not blindly turn into a blocked path.

### Case 7 — The servo scan is inaccurate or incomplete

**What happens:**  
The servo may not reach the commanded angle, the sensor may wobble, or the code may measure before the servo settles. The robot then compares misleading readings.

**How to reduce the risk:**
- Allow a short settling delay after each servo movement.
- Keep the sensor and servo mechanically secure.
- Verify left, centre, and right angles physically.
- Avoid forcing the servo beyond its safe range.
- If the servo fails, use a conservative stop behavior instead of assuming the scan succeeded.

**Test:** Command each scan angle repeatedly and confirm the sensor points where expected before taking a reading.

### Case 8 — No echo is interpreted as “infinite distance” or “clear path”

**What happens:**  
`pulseIn()` can time out when no echo returns. If the code treats that timeout as a very large distance, the robot may drive forward even though the reason for the missing echo is unknown.

**How to reduce the risk:**
- Use a timeout when measuring the echo pulse.
- Validate the returned duration and resulting distance.
- Keep “no valid reading” separate from “valid reading of a far-away obstacle.”
- For repeated invalid readings, stop and retry or signal a fault.

**Test:** Point the sensor into open space and toward a surface that does not return a clear echo. Check that invalid readings do not automatically command forward motion.

### Case 9 — Electrical noise, loose wiring, or a weak battery causes erratic behavior

**What happens:**  
Motor current can cause voltage dips or electrical noise. Loose connections, a weak battery, or poor grounding can reset the controller or corrupt sensor readings.

**How to reduce the risk:**
- Secure all connections and inspect them before each test.
- Follow the motor-driver and controller voltage requirements.
- Use a suitable power arrangement for the motors and controller, with a common ground where the circuit design requires it.
- Keep sensor signal wires away from high-current motor wiring where practical.
- Do not power motors directly from an Arduino I/O pin.
- Stop testing if the controller resets, the driver overheats, or wiring becomes hot.

**Test:** Observe whether sensor readings or controller behavior change when the motors start. Check battery voltage under load and inspect connections.

### Case 10 — Motor driver or motor wiring is incorrect

**What happens:**  
The robot may move backward when told to move forward, spin unexpectedly, or fail to turn because motor direction pins are reversed or a connection is loose.

**How to reduce the risk:**
- Test each motor at low speed with the wheels lifted off the ground.
- Confirm left/right motor mapping and direction before running obstacle logic.
- Check the motor driver's enable pins and power wiring.
- Keep wheels off the ground during initial direction tests.

**Test:** Run forward, reverse, left, right, and stop commands individually and record the actual motion.

### Case 11 — The robot gets stuck in a loop

**What happens:**  
The robot repeatedly detects an obstacle, turns, sees the same obstacle, and repeats the same action forever.

**How to reduce the risk:**
- Count repeated failed avoidance attempts.
- Change the recovery action after a failed turn.
- Add a maximum recovery count and stop safely when it is reached.
- Use a short reverse movement only when the rear path is known to be clear.

**Test:** Place the robot in a corner or a U-shaped arrangement of boxes. Confirm it eventually stops or changes strategy instead of looping forever.

### Case 12 — The robot reverses into an obstacle

**What happens:**  
A common recovery routine reverses without checking behind the robot. This can cause a collision with an object behind it.

**How to reduce the risk:**
- Do not assume the rear path is clear.
- Use a rear sensor or bumper if reverse movement must be reliable.
- Otherwise, limit reverse movement to carefully supervised tests and use a conservative stop when the rear is unknown.

**Test:** Put an object behind the robot and trigger the recovery routine. Confirm the robot does not reverse blindly.

### Case 13 — The sensor sees a clear path, but the chassis or wheels still hit the obstacle

**What happens:**  
The sensor measures distance along its own beam, not the entire width of the robot. An obstacle near one front corner can be outside the beam while still intersecting the chassis or wheel path.

**How to reduce the risk:**
- Account for the robot's full width and turning radius.
- Keep a safety margin around measured obstacles.
- Test narrow gaps and corner approaches.
- Add side sensing or bump switches if the robot must navigate close spaces.

**Test:** Approach a box at a slight angle and check whether a wheel or chassis corner contacts it before the sensor reports a close obstacle.

### Case 14 — The robot approaches a drop, stair edge, or hole

**What happens:**  
A forward-facing ultrasonic sensor is not a reliable cliff detector. It may see the floor ahead, miss the edge, or return inconsistent readings.

**How to reduce the risk:**
- Do not use this robot near stairs, ledges, balconies, or drop-offs.
- For floor-edge detection, use suitable downward-facing cliff sensors and test them separately.
- Treat cliff detection as a separate safety function, not as a feature provided by the front ultrasonic sensor.

**Test:** Only test a dedicated cliff-sensing setup on a low, controlled test platform with a physical catch/stop. Do not test by driving toward real stairs.

### Case 15 — Sensor readings are taken too soon after a previous measurement

**What happens:**  
Echoes from a previous ping or nearby surfaces may interfere with the next measurement, causing unstable distances.

**How to reduce the risk:**
- Leave an appropriate interval between ultrasonic measurements.
- Avoid taking many pings back-to-back without a delay.
- Filter readings and reject implausible jumps.

**Test:** Compare readings taken with a reasonable interval against readings taken rapidly. Use the timing recommended for the specific sensor module.

### Case 16 — Threshold values do not match the real robot

**What happens:**  
A fixed threshold (for example, 20 cm) may be too small at high speed or unnecessarily large at low speed. Sensor mounting and battery condition also affect real behavior.

**How to reduce the risk:**
- Treat example thresholds as starting points, not universal values.
- Measure sensor accuracy and stopping distance on the actual robot.
- Tune the threshold at low speed in a clear test area.
- Document the final threshold and the conditions under which it was tested.

**Test:** Repeat obstacle approaches at different speeds and battery states. Verify that the robot stops with a margin before contact.

---

## 3. Recommended control logic

A conservative high-level flow is:

1. **Start safely:** Keep the robot stopped until the controller and sensor are ready.
2. **Read the sensor:** Take a measurement with a timeout.
3. **Validate the result:** Distinguish a valid distance from a timeout or implausible reading.
4. **Apply a safety rule:** If the reading is invalid or the obstacle is within the calibrated stopping margin, stop.
5. **Scan if supported:** Stop, point the servo left and right, allow it to settle, and take valid readings.
6. **Choose cautiously:** Turn only toward a direction with sufficient measured clearance.
7. **Recover safely:** Avoid blind reverse motion; limit repeated recovery attempts.
8. **Recheck continuously:** Measure again while moving and stop if the sensor becomes unreliable.

**Important:** This is design guidance, not a drop-in replacement for the robot's actual Arduino sketch. Pin assignments, motor-driver logic, servo angles, and thresholds must match the real build.

## 4. Test checklist for GitHub

Use this checklist when validating changes:

- [ ] Sensor is firmly mounted and points in the intended direction.
- [ ] Flat obstacle directly ahead is detected.
- [ ] Obstacles at left and right angles are tested.
- [ ] Low and high obstacles are tested.
- [ ] Narrow, soft, and angled surfaces are tested.
- [ ] Invalid/no-echo readings do not cause automatic forward motion.
- [ ] Robot stopping distance is measured at each intended speed.
- [ ] Left/right scan angles are verified if a servo is installed.
- [ ] Robot does not blindly turn toward a blocked side.
- [ ] Robot does not reverse into an unknown rear path.
- [ ] Robot does not loop forever in a corner.
- [ ] Motor directions and stop behavior are verified.
- [ ] Battery and wiring remain stable while motors run.
- [ ] The robot is kept away from stairs and drop-offs unless dedicated cliff sensors are installed and validated.

## 5. Suggested test log

Record real observations rather than claiming a test passed without evidence.

| Test ID | Scenario | Expected behavior | Actual result | Pass/Fail | Fix or notes |
|---|---|---|---|---|---|
| T01 | Flat box in front | Stop before contact | Not tested | Pending | |
| T02 | Object off-centre | Detect or stop conservatively | Not tested | Pending | |
| T03 | Low obstacle | Detect, or document limitation | Not tested | Pending | |
| T04 | No valid echo | Stop/retry safely | Not tested | Pending | |
| T05 | Both sides blocked | Stop or bounded recovery | Not tested | Pending | |
| T06 | Obstacle behind robot | No blind reverse | Not tested | Pending | |
| T07 | Corner/U-shaped trap | Avoid infinite loop | Not tested | Pending | |
| T08 | High motor load | No reset or erratic movement | Not tested | Pending | |

## 6. Known limitations

- A single forward-facing ultrasonic sensor cannot reliably detect every obstacle or cover every direction.
- Ultrasonic sensors do not identify what an object is.
- A distance reading does not prove that a complete turning path is clear.
- A servo scan samples directions one at a time; it is not continuous 360° awareness.
- This project is an educational prototype, not a safety-rated autonomous navigation system.
- Results depend on sensor placement, code, speed, motor driver, battery, floor surface, and obstacle material.

## 7. Future improvements

Possible upgrades, depending on budget and project goals:

- Add side or rear distance sensors for wider coverage.
- Add bumper switches as a last-resort contact detector.
- Add downward-facing cliff sensors if floor-edge detection is required.
- Add a status LED or buzzer pattern for sensor faults and repeated recovery.
- Log distance readings and state transitions over Serial for debugging.
- Use a state machine (`DRIVE`, `STOP`, `SCAN`, `TURN`, `RECOVER`, `FAULT`) rather than a long sequence of nested conditions.
- Add a timeout and retry limit to every movement/recovery action.

---

### Disclaimer

This file documents common failure modes and practical mitigations. It does not certify the robot as safe. Validate each change on the actual hardware, at low speed, with supervision, and in a controlled environment.
