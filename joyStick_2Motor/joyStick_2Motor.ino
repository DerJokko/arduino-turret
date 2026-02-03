#include <Stepper.h>

/* ===== Motor-Konfiguration ===== */
const int STEPS_PER_REV = 2048;   // 28BYJ-48

Stepper motorHorizontal(STEPS_PER_REV, 8, 10, 9, 11);
Stepper motorVertical(STEPS_PER_REV, 4, 5, 6, 7);

/* ===== Joystick & Input ===== */
const int VRX_PIN = A0;
const int VRY_PIN = A1;
const int FIRE_PIN = 2;

/* ===== Allgemeine Einstellungen ===== */
const int IDLE_VALUE = 512;
const int DEADZONE = 20;
const int MAX_RPM = 16;
const int STEP_MULTIPLIER = 4;

/* ===== Vertikaler Motor – Winkelbegrenzung ===== */
const int VERTICAL_RANGE_DEG = 180;     // <<< EINSTELLBAR
const int VERTICAL_MAX_STEPS =
  (STEPS_PER_REV * VERTICAL_RANGE_DEG) / 360;

int verticalPositionSteps = VERTICAL_MAX_STEPS / 2; // Start in Mittelposition

/* ================================================= */

void setup() {
  Serial.begin(9600);
  pinMode(FIRE_PIN, INPUT);

  motorHorizontal.setSpeed(MAX_RPM);
  motorVertical.setSpeed(MAX_RPM);

  Serial.println("System gestartet");
}

void loop() {
  int joyX = analogRead(VRX_PIN);
  int joyY = analogRead(VRY_PIN);

  moveHorizontal(joyX);
  moveVertical(joyY);

  if (digitalRead(FIRE_PIN) == HIGH) {
    Serial.println("🔥 FIRE!");
  }

  delay(20);
}

/* ===== Horizontalbewegung (endlos) ===== */
void moveHorizontal(int input) {
  int delta = input - IDLE_VALUE;
  if (abs(delta) < DEADZONE) return;

  float speedFraction = (float)delta / 512.0;
  int rpm = speedFraction * MAX_RPM;

  int direction = (rpm > 0) ? 1 : -1;
  int steps = abs(rpm) * STEP_MULTIPLIER;

  motorHorizontal.setSpeed(abs(rpm));
  motorHorizontal.step(steps * direction);
}

/* ===== Vertikalbewegung (begrenzt) ===== */
void moveVertical(int input) {
  int delta = input - IDLE_VALUE;
  if (abs(delta) < DEADZONE) return;

  float speedFraction = (float)delta / 512.0;
  int rpm = speedFraction * MAX_RPM;

  int direction = (rpm > 0) ? 1 : -1;
  int steps = abs(rpm) * STEP_MULTIPLIER;

  // Begrenzung prüfen
  int newPosition = verticalPositionSteps + (steps * direction);

  if (newPosition < 0) {
    steps = verticalPositionSteps; // nur bis Anschlag
    direction = -1;
  } 
  else if (newPosition > VERTICAL_MAX_STEPS) {
    steps = VERTICAL_MAX_STEPS - verticalPositionSteps;
    direction = 1;
  }

  if (steps <= 0) return;

  motorVertical.setSpeed(abs(rpm));
  motorVertical.step(steps * direction);

  verticalPositionSteps += steps * direction;
}
