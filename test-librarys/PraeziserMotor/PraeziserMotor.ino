#include <Stepper.h>

const int stepsPerRevolution = 2048;  // 28BYJ-48
Stepper motorHorizontal(stepsPerRevolution, 8, 10, 9, 11);

// --- Konfiguration ---
int idleValue = 512;   // Wert, bei dem Motor stillsteht
int speedInput = 0;  // Testwert: 0-1024
int maxRPM = 16;       // maximale Drehgeschwindigkeit
int puffer = 20;       // Puffer, bei dem der Motor nicht dreht

void setup() {
  Serial.begin(9600);
}

void loop() {
  MoveHorizontal();
}

void MoveHorizontal() {
  int delta = speedInput - idleValue;  // Differenz vom Idle

  if (abs(delta) < puffer) return;  // Motor still

  // Berechne proportionale Geschwindigkeit
  // delta negativ -> links, positiv -> rechts
  float speedFraction = (float)delta / 512.0;  // Werte von -1 bis +1
  int motorRPM = speedFraction * maxRPM;       // -maxRPM bis +maxRPM

  // Drehrichtung
  int direction = (motorRPM > 0) ? 1 : -1;

  // Multiplikator anpassen für Kleinschrittigkeit
  const int stepMultiplier = 4;
  // Schrittzahl pro Loop (für sichtbare Bewegung)
  int stepsToMove = abs(motorRPM) * stepMultiplier;

  // Setze Geschwindigkeit und bewege Motor
  motorHorizontal.setSpeed(abs(motorRPM));
  motorHorizontal.step(stepsToMove * direction);

  // Optional: Debug-Ausgabe
  /*
  Serial.print("speedInput: ");
  Serial.print(speedInput);
  Serial.print("  delta: ");
  Serial.print(delta);
  Serial.print("  motorRPM: ");
  Serial.println(motorRPM);
  */

  delay(20);  // kurze Pause für sanfte Bewegung
}