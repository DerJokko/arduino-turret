
/*
Code für Uno mit Joystick und Sender
 - left/right 
 - fire button

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


| Joystick | Arduino           | Erklärung        |
| -------- | ----------------- | ---------------- |
| GND      | GND               |                  |
| +5V      | 5V                |                  |
| VRx      | A0                |                  |
| VRy      | A1                |                  |
| SW       | Digital           | Optional         |


| Button   | Arduino           | Erklärung        |
| -------- | ----------------- | ---------------- |
| Cable1   | GND               |                  |
| Cable2   | Pin 8             |                  |

*/


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);  // CE, CSN

const byte address[6] = "00001";

const int VRx_PIN = A0;
const int VRy_PIN = A1;
const int FIRE_BTN = 8;
const int DEADZONE = 10;

float smoothX = 0;
float smoothY = 0;

const float alpha = 0.2;  // 0.0–1.0 (kleiner = smoother)

// ===== Paket =====
struct ControlData {
  int16_t x;
  int16_t y;
  uint8_t fire;
};

ControlData data;

void setup() {
  Serial.begin(9600);

  pinMode(FIRE_BTN, INPUT_PULLUP);

  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {

  // ===== X =====
  int rawX = analogRead(VRx_PIN);
  int x = rawX - 512;
  if (abs(x) < DEADZONE) x = 0;

  // Glättung
  smoothX = smoothX + alpha * (x - smoothX);
  data.x = -(int)smoothX;

  // ===== Y =====
  int rawY = analogRead(VRy_PIN);
  int y = rawY - 512;
  if (abs(y) < DEADZONE) y = 0;

  smoothY = smoothY + alpha * (y - smoothY);
  data.y = (int)smoothY;

  // ===== FIRE =====
  data.fire = (digitalRead(FIRE_BTN) == LOW) ? 1 : 0;

  // ===== SENDEN =====
  radio.write(&data, sizeof(data));

  // ===== DEBUG =====
  Serial.print("X: ");
  Serial.print(data.x);
  Serial.print(" | Y: ");
  Serial.print(data.y);
  Serial.print(" | Fire: ");
  Serial.println(data.fire ? "YES" : "NO");

  delay(20);
}