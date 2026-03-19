
/*
Code für Uno mit Motor und Sender (Unteres Turretmodul)
 - left/right 

| NRF24L01 | Arduino           | Erklärung        |
| -------- | ----------------- | ---------------- |
|   VCC    | 3.3V              | NIEMALS 5V       |
|   GND    | GND               | Gemeinsame Masse |
|   CE     | Pin 9             | Frei wählbar     |
|   CSN    | Pin 10            | Frei wählbar     |
|   SCK    | Pin 13            | SPI-Clock        |
|   MOSI   | Pin 11            | SPI-Daten        |
|   MISO   | Pin 12            | SPI-Daten        |
|   IRQ    | nicht anschließen | Optional         |


| Treiber  | Arduino           | Erklärung        |
| -------- | ----------------- | ---------------- |
| -        | GND               |                  |
| +        | 5V                |                  |
| 1N1      | Pin 4             |                  |
| 1N1      | Pin 5             |                  |
| 1N1      | Pin 6             |                  |
| 1N1      | Pin 7             |                  |

*/


#include <Stepper.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// ---------- Funk ----------
RF24 radio(9, 10);
const byte address[6] = "00001";

int16_t receivedValue = 0;
unsigned long lastReceiveTime = 0;
const unsigned long TIMEOUT_MS = 200; // Funk-Timeout

// ---------- Stepper ----------
const int stepsPerRevolution = 2048;
Stepper motorHorizontal(stepsPerRevolution, 4, 6, 5, 7);

// --- Motor-Konfig ---
int maxRPM = 15;
const int stepMultiplier = 4;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
}

void loop() {

  // 📡 Empfang
  if (radio.available()) {
    radio.read(&receivedValue, sizeof(receivedValue));
    lastReceiveTime = millis();

    Serial.print("Received: ");
    Serial.println(receivedValue);
  }

  // 🚨 Failsafe: kein Signal → STOP
  if (millis() - lastReceiveTime > TIMEOUT_MS) {
    receivedValue = 0;
  }

  MoveHorizontal(receivedValue);
  delay(5);
}

void MoveHorizontal(int value) {

  // Wert 0 → Motor aus
  if (value == 0) return;

  // Normieren auf -1 … +1
  float speedFraction = (float)value / 512.0;
  int motorRPM = speedFraction * maxRPM;

  int direction = (motorRPM > 0) ? 1 : -1;
  int stepsToMove = abs(motorRPM) * stepMultiplier;

  motorHorizontal.setSpeed(abs(motorRPM));
  motorHorizontal.step(stepsToMove * direction);
}
