/*
  Autonomous Obstacle Avoidance Robot
  Target: Arduino Uno / ATmega328P
  Assumed hardware: HC-SR04, L298N dual H-bridge, active buzzer,
                    two motor groups (left side and right side).

  Behaviour:
    Distance > 35 cm : move forward at PWM 200; buzzer OFF
    Distance 20-35 cm: move forward at PWM 130; slow pulsed beep
    Distance < 20 cm : buzzer ON; pivot left (left side reverse,
                       right side forward) for a fixed duration

  IMPORTANT:
  1. The wiring table in docs/WIRING.md must match your physical wiring.
  2. Check motor direction with the wheels lifted before floor testing.
  3. Two motors connected in parallel to one L298N channel draw their
     combined current. Confirm that this is within the driver's rating.
  4. A missing ultrasonic echo is treated as an unknown path, so the
     robot stops rather than continuing blindly.
*/

#include <Arduino.h>

// HC-SR04 pins
const uint8_t TRIG_PIN = 12;
const uint8_t ECHO_PIN = 11;

// Active buzzer signal pin
const uint8_t BUZZER_PIN = 2;

// L298N: left motor group
const uint8_t ENA_PIN = 5;  // PWM
const uint8_t IN1_PIN = 7;
const uint8_t IN2_PIN = 8;

// L298N: right motor group
const uint8_t ENB_PIN = 6;  // PWM
const uint8_t IN3_PIN = 9;
const uint8_t IN4_PIN = 10;

// Distance thresholds (centimetres)
const float CAUTION_DISTANCE_CM = 35.0;
const float STOP_DISTANCE_CM = 20.0;

// Motor PWM values: 0-255
const int CRUISE_SPEED = 200;
const int CAUTION_SPEED = 130;
const int TURN_SPEED = 180;

// Timing
const unsigned long ECHO_TIMEOUT_US = 30000UL;
const unsigned long CAUTION_BEEP_INTERVAL_MS = 350UL;
const unsigned long TURN_DURATION_MS = 350UL;

unsigned long lastBuzzerToggleMs = 0;
bool buzzerIsOn = false;

void setOneMotor(int speedValue, uint8_t enablePin,
                 uint8_t inputPinA, uint8_t inputPinB) {
  // Restrict speed to the valid analogWrite range.
  speedValue = constrain(speedValue, -255, 255);

  if (speedValue > 0) {
    digitalWrite(inputPinA, HIGH);
    digitalWrite(inputPinB, LOW);
    analogWrite(enablePin, speedValue);
  } else if (speedValue < 0) {
    digitalWrite(inputPinA, LOW);
    digitalWrite(inputPinB, HIGH);
    analogWrite(enablePin, -speedValue);
  } else {
    analogWrite(enablePin, 0);
    digitalWrite(inputPinA, LOW);
    digitalWrite(inputPinB, LOW);
  }
}

// Positive = forward, negative = reverse, zero = stop.
// This assumes both sides have been wired so positive speed moves forward.
void drive(int leftSpeed, int rightSpeed) {
  setOneMotor(leftSpeed, ENA_PIN, IN1_PIN, IN2_PIN);
  setOneMotor(rightSpeed, ENB_PIN, IN3_PIN, IN4_PIN);
}

void stopMotors() {
  drive(0, 0);
}

void buzzerOff() {
  digitalWrite(BUZZER_PIN, LOW);
  buzzerIsOn = false;
  lastBuzzerToggleMs = millis();
}

void updateCautionBuzzer() {
  const unsigned long now = millis();

  if (now - lastBuzzerToggleMs >= CAUTION_BEEP_INTERVAL_MS) {
    lastBuzzerToggleMs = now;
    buzzerIsOn = !buzzerIsOn;
    digitalWrite(BUZZER_PIN, buzzerIsOn ? HIGH : LOW);
  }
}

float measureDistanceCm() {
  // Send the HC-SR04 trigger pulse.
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // pulseIn returns 0 if no echo arrives before the timeout.
  const unsigned long echoDurationUs =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (echoDurationUs == 0) {
    return -1.0;  // Invalid / missing echo
  }

  // Common HC-SR04 approximation: distance in cm = duration / 58.
  return static_cast<float>(echoDurationUs) / 58.0;
}

void avoidObstacle() {
  // Audible warning during the evasion manoeuvre.
  digitalWrite(BUZZER_PIN, HIGH);
  buzzerIsOn = true;

  stopMotors();
  delay(120);

  // Pivot left: left group reverses, right group moves forward.
  // If your physical motor wiring reverses this result, swap the
  // motor leads for that side or invert its direction logic.
  drive(-TURN_SPEED, TURN_SPEED);
  delay(TURN_DURATION_MS);

  stopMotors();
  buzzerOff();
  delay(100);
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  buzzerOff();
  stopMotors();

  Serial.begin(9600);
  delay(300);
}

void loop() {
  const float distanceCm = measureDistanceCm();

  Serial.print(F("Distance: "));
  if (distanceCm < 0.0) {
    Serial.println(F("NO ECHO - stopping for safety"));
    stopMotors();
    buzzerOff();
    delay(80);
    return;
  }

  Serial.print(distanceCm, 1);
  Serial.println(F(" cm"));

  if (distanceCm < STOP_DISTANCE_CM) {
    avoidObstacle();
  } else if (distanceCm <= CAUTION_DISTANCE_CM) {
    drive(CAUTION_SPEED, CAUTION_SPEED);
    updateCautionBuzzer();
  } else {
    drive(CRUISE_SPEED, CRUISE_SPEED);
    buzzerOff();
  }

  // Small pause between readings.
  delay(60);
}
