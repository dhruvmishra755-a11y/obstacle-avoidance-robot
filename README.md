# 🚗 Autonomous Obstacle Avoidance Robot

### Intelligent Navigation · Embedded Systems · Robotics

![Project](https://img.shields.io/badge/Project-Autonomous%20Robotics-blue?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![Language](https://img.shields.io/badge/Language-C%2FC%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Status](https://img.shields.io/badge/Status-Hardware%20Built-success?style=for-the-badge)

**A compact robotic vehicle that uses ultrasonic distance sensing and embedded control logic to respond to nearby obstacles.**

---

## 📸 Project Build

The repository includes an assembly-stage photograph as a record of the hardware build. Replace it with a clear photograph of the completed robot when available.

![Robot assembly](assets/robot-assembly.jpg)

---

## 📌 Table of Contents

- [Overview](#-overview)
- [Objectives](#-objectives)
- [Key Features](#-key-features)
- [How It Works](#-how-it-works)
- [System Architecture](#-system-architecture)
- [Hardware Components](#-hardware-components)
- [Ultrasonic Distance Calculation](#-ultrasonic-distance-calculation)
- [Firmware Behaviour](#-firmware-behaviour)
- [Power and Electrical Safety](#-power-and-electrical-safety)
- [Repository Structure](#-repository-structure)
- [Getting Started](#-getting-started)
- [Testing and Troubleshooting](docs/TESTING_AND_TROUBLESHOOTING.md)
- [Limitations](#-limitations)
- [Future Improvements](#-future-improvements)
- [Learning Outcomes](#-learning-outcomes)
- [Author](#-author)
- [License](#-license)

## 🔍 Overview

The **Autonomous Obstacle Avoidance Robot** is an embedded-systems project combining ultrasonic sensing, microcontroller programming, DC motor control, and mechanical assembly.

An HC-SR04-compatible ultrasonic sensor measures distance to nearby objects. An Arduino Uno-compatible controller processes the measurement and runs the movement logic. An L298N dual H-bridge motor driver controls the left and right motor groups, while an active buzzer provides audible feedback.

The robot is assembled on a four-wheel chassis with a concealed lithium-ion battery pack. The concealment and compact assembly improve the finished appearance, but the battery must remain electrically insulated, adequately protected, and accessible for safe maintenance.

> **Hardware/firmware assumptions:** The supplied sketch targets an Arduino Uno, HC-SR04 sensor, L298N driver, active buzzer, and the exact pin mapping in [`docs/WIRING.md`](docs/WIRING.md). The supplied code has not been verified against your physical wiring. Before powering the motors, make the wiring match that guide or change the pin constants in the sketch.

## 🎯 Objectives

- Measure nearby obstacle distance using ultrasound.
- Process sensor readings on a microcontroller.
- Control DC motor direction and speed through a motor driver.
- Use threshold-based logic to adjust movement.
- Combine power, wiring, sensors, control electronics, and chassis into one robotic platform.
- Practise embedded programming, hardware integration, testing, and troubleshooting.

## ✨ Key Features

| Feature | Description |
|---|---|
| 📡 Ultrasonic sensing | Measures approximate distance to objects in front of the sensor |
| 🧠 Embedded decision logic | Selects a movement response based on distance thresholds |
| ⚙️ Dual-channel motor control | Controls left and right motor groups through an L298N-compatible driver |
| 🚗 Four-wheel chassis | Provides a mobile platform |
| 🔋 Concealed lithium-ion pack | Keeps the battery arrangement compact; electrical protection remains essential |
| 🔔 Audible feedback | Uses an active buzzer for proximity and evasion indications |
| 🧰 Documented firmware | Includes an Arduino sketch, pin map, and testing notes |

## ⚙️ How It Works

1. **Sense:** The controller triggers the ultrasonic sensor.
2. **Measure:** The sensor returns an echo pulse whose duration indicates the round-trip travel time.
3. **Calculate:** The controller estimates the one-way distance.
4. **Compare:** The distance is checked against the configured thresholds.
5. **Act:** The motor driver changes the movement, and the buzzer signals the operating state.
6. **Repeat:** The controller measures again and continues the control loop.

### Simplified Control Flow

```text
             ┌─────────────────────┐
             │      POWER ON       │
             └──────────┬──────────┘
                        ▼
             ┌─────────────────────┐
             │ Initialise Hardware │
             └──────────┬──────────┘
                        ▼
             ┌─────────────────────┐
             │ Measure Distance    │
             └──────────┬──────────┘
                        ▼
             ┌─────────────────────┐
             │ Valid Echo Received?│
             └───────┬───────┬─────┘
                     │       │
                    NO      YES
                     │       ▼
                     │  ┌────────────────────┐
                     │  │ Compare Thresholds │
                     │  └─────────┬──────────┘
                     │            ▼
                     │  ┌────────────────────┐
                     │  │ Forward / Caution /│
                     │  │ Evasion Response   │
                     │  └─────────┬──────────┘
                     │            │
                     ▼            ▼
             ┌─────────────────────────┐
             │ Stop Safely / Repeat    │
             └─────────────────────────┘
```

## 🏗️ System Architecture

```text
    HC-SR04 Ultrasonic Sensor
                │ echo / trigger
                ▼
      Arduino-Compatible MCU
       distance + decision logic
                │ direction / PWM
                ▼
          L298N Driver  ◄──── Battery / motor supply
             │     │
             ▼     ▼
       Left motors  Right motors
             └──┬──┘
                ▼
          Robot movement

    Active buzzer ◄──── MCU digital signal
```

- **Sensing subsystem:** measures distance.
- **Processing subsystem:** calculates distance and evaluates thresholds.
- **Actuation subsystem:** drives the motors through the H-bridge.
- **Feedback subsystem:** the buzzer provides a simple audible indication.
- **Power subsystem:** supplies the logic and motor circuits using suitable voltage regulation and protection.

## 🔩 Hardware Components

| Component | Function |
|---|---|
| Arduino Uno-compatible board | Runs the firmware and produces control signals |
| HC-SR04 ultrasonic sensor | Measures distance to obstacles |
| L298N dual H-bridge driver | Switches motor direction and controls speed through PWM inputs |
| Four DC geared motors / four-wheel chassis | Provide locomotion |
| Lithium-ion battery pack | Supplies energy to the robot |
| Active buzzer | Produces audible alerts |
| Jumper wires, connectors, standoffs | Provide electrical connections and mechanical mounting |

Check the labels on your actual boards. The exact board revision and motor model should be recorded only after verification.

## 📡 Ultrasonic Distance Calculation

The sensor measures the time taken for the ultrasonic pulse to travel to an object and return.

**Distance = (speed of sound × round-trip time) ÷ 2**

For an HC-SR04-style sensor, a commonly used approximation is:

```text
distance_cm ≈ echo_duration_microseconds / 58
```

**Example:** an echo lasting 1,160 μs corresponds to approximately:

```text
1160 / 58 = 20 cm
```

This is an estimate. Soft, angled, narrow, or irregular objects and environmental conditions can affect the reading.

## 🧠 Firmware Behaviour

The included sketch uses the following illustrative thresholds and actions:

| Distance reading | Firmware action |
|---|---|
| More than 35 cm | Move forward with PWM 200; buzzer off |
| 20–35 cm | Move forward with PWM 130; buzzer pulses |
| Less than 20 cm | Stop briefly, activate buzzer, pivot left for a fixed duration |
| No echo received | Stop the motors; treat the distance as unknown |

These are **configured values, not measured performance guarantees**. Test and tune them for your robot's mass, motor response, surface, and sensor mounting.

The turning action uses a fixed duration rather than wheel-encoder feedback, so it will not guarantee an exact turning angle. The code assumes the left and right motor groups are wired so positive speed moves both sides forward; verify with the wheels raised.

## 🔋 Power and Electrical Safety

The project notes describe a two-cell series-connected 18650 lithium-ion pack (2S): **7.4 V nominal and 8.4 V fully charged**. Verify that this is the pack you actually installed.

- Never connect a lithium-ion pack directly to the Arduino 5V pin.
- Use suitable protection for the cells and a charger designed for the pack configuration.
- Check motor voltage ratings. A 2S pack is not automatically suitable for motors rated 3–6 V; the L298N is not a precision voltage regulator.
- Confirm the combined current of the motors on each driver channel stays within the driver's safe thermal and current limits. Two motors in parallel draw more current than one.
- Keep battery terminals insulated and secured against vibration.
- Ensure the driver and regulator can dissipate heat.
- Make the battery accessible for inspection, even if concealed.
- Disconnect power before changing wiring.

See [`docs/WIRING.md`](docs/WIRING.md) for important power and common-ground notes.

## 📁 Repository Structure

```text
obstacle-avoidance-robot/
├── README.md
├── LICENSE
├── .gitignore
├── src/
│   └── obstacle_avoidance_robot.ino
├── docs/
│   ├── WIRING.md
│   └── TESTING_AND_TROUBLESHOOTING.md
└── assets/
    └── robot-assembly.jpg
```

## 🚀 Getting Started

1. Download or clone this repository.
2. Open `src/obstacle_avoidance_robot.ino` in the Arduino IDE.
3. Follow the matching pin map in [`docs/WIRING.md`](docs/WIRING.md).
4. Select **Arduino Uno** (or the correct compatible board) and the appropriate port in the IDE.
5. Upload the sketch.
6. Open Serial Monitor at **9600 baud** to see the distance readings.
7. First test with the wheels lifted and then use low-speed tests in an open area.

**Do not upload and run the sketch blindly if your wiring differs.** Matching the code's pin constants to the actual circuit is required.

## ⚠️ Limitations

- Ultrasonic readings can be unreliable for some materials, surfaces, and angles.
- One forward-facing sensor cannot see every direction.
- A fixed-duration turn does not measure or guarantee the actual angle.
- Motor mismatch, battery level, floor friction, and chassis alignment affect movement.
- The robot does not perform global path planning or know its exact position.
- This is an educational prototype, not a safety-certified autonomous vehicle.

## 🚀 Future Improvements

- Mount the ultrasonic sensor on a servo to scan left and right.
- Add wheel encoders for feedback on wheel rotation.
- Calibrate each motor's speed and turning behaviour.
- Add a battery-voltage monitor.
- Use an ESP32 for wireless telemetry.
- Experiment with camera-based object detection.
- Explore ROS 2, mapping, and localisation after the basic platform is reliable.

These are potential extensions, not features claimed to be present in the current build.

## 🧠 Learning Outcomes

- Microcontroller-based system design.
- Ultrasonic sensor interfacing and time-of-flight calculations.
- Embedded C/C++ programming.
- H-bridge motor control and PWM.
- Battery-powered electronics and safe power distribution.
- Hardware integration, wiring, testing, and troubleshooting.
- Threshold-based decision logic and autonomous response.

## 👨‍💻 Author

**Dhruv Mishra**  
B.Tech — Computer Science Engineering (AI & ML)

Interested in robotics, embedded systems, autonomous navigation, and practical engineering projects.

**GitHub:** [@dhruvmishra755-a11y](https://github.com/dhruvmishra755-a11y)

## 📄 License

This repository includes the MIT License. See [`LICENSE`](LICENSE) for the full terms.

---

<div align="center">

**BUILD · TEST · IMPROVE**

*Engineering intelligent systems through practical experimentation.*

</div>
