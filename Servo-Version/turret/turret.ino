
/*
Empfänger mit:
- Servo
- NRF24L01 Empfang von X, Y + Fire

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


| Servos   | Arduino           | Erklärung        |
| -------- | ----------------- | ---------------- |
| braun    | GND               |                  |
| rot      | 5V                |                  |
| x        | Pin 6             | R/L              |
| y        | Pin 5             | O/U              |
| Fire     | Pin 3             |                  |


*/

#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// ================= NRF24 =================

RF24 radio(9, 10);
const byte address[6] = "00001";

struct ControlData {
  int16_t x;
  int16_t y;
  uint8_t fire;
};

ControlData data;

// ================= SERVOS =================

Servo servoX;     // Rechts/Links
Servo servoY;     // Oben/Unten
Servo servoFire;  // Schuss

#define PIN_X 6
#define PIN_Y 5
#define PIN_FIRE 3

int posX = 90;
int posY = 90;

const int FIRE_SHOOT = 25;
const int FIRE_REST = 85;

bool lastFireState = false;
bool isFiring = false;

// ================= SETUP =================

void setup() {
  Serial.begin(9600);

  servoX.attach(PIN_X);
  servoY.attach(PIN_Y);
  servoFire.attach(PIN_FIRE);

  servoX.write(posX);
  servoY.write(posY);
  servoFire.write(FIRE_REST);

  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(0, address);
  radio.startListening();
}

// ================= LOOP =================

void loop() {

  if (radio.available()) {

    radio.read(&data, sizeof(data));

    Serial.print("X: ");
    Serial.print(data.x);
    Serial.print(" | Y: ");
    Serial.print(data.y);
    Serial.print(" | Fire: ");
    Serial.println(data.fire);

    // ======== X SERVO ========
    posX = map(data.x, -512, 512, 0, 180);
    posX = constrain(posX, 0, 180);
    servoX.write(posX);

    // ======== Y SERVO ========
    posY = map(data.y, -512, 512, 60, 160);
    posY = constrain(posY, 0, 180);
    servoY.write(posY);

    // ======== FIRE ========
    bool currentFire = (data.fire == 1);

    if (currentFire && !lastFireState && !isFiring) {
      fire();
    }

    lastFireState = currentFire;
  }
}

// ================= FIRE =================

void fire() {
  isFiring = true;

  Serial.println("FIRE!");

  servoFire.write(FIRE_SHOOT);
  delay(200);

  servoFire.write(FIRE_REST);
  delay(200);

  isFiring = false;
}