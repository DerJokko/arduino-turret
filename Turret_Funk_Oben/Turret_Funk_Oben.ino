
/*
Code für Uno mit Display, DCs, Servo, VRotator und Sender

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


| Display  | Arduino           | Erklärung        |
| -------- | ----------------- | ---------------- |
| GND      | GND               |                  |
| VCC      | 5V                |                  |
| SDA      | A4                |                  |
| SCL      | A5                |                  |


| Servo    | Arduino           | Erklärung        |
| -------- | ----------------- | ---------------- |
| braun    | GND               |                  |
| rot      | 5V                |                  |
| gelb     | Pin 8             |                  |


| <Dcs>    | Arduino           | Erklärung        |
| -------- | ----------------- | ---------------- |
| GND      | GND               |                  |
| M1A      | Pin 4             |                  |
| M1B      | Pin 5             |                  |
| M2A      | Pin 6             |                  |
| M2B      | Pin 7             |                  |


*/

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;

// ================= SERVO =================

int angle = 10;
bool servoDirectionUp = true;

unsigned long lastServoUpdate = 0;
const unsigned long servoInterval = 15; // ms

// ================= SETUP =================

void setup() {

  InitializeDCMotor();

  servo.attach(8);
  servo.write(angle);

  lcd.init();
  lcd.backlight();

  loadEyeChars();
  drawEyes();
}

// ================= LOOP =================

void loop() {
  UpdateServo();
}


// ================= DC MOTOR INIT =================

void InitializeDCMotor() {
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(4, LOW);
}

// ================= AUGE CUSTOM CHARS =================

// obere Reihe
byte eyeTopLeft[8] = {
  B00000,B00000,B00000,B00000,
  B00000,B00001,B00011,B01111
};

byte eyeTopMid[8] = {
  B00000,B00000,B00000,B00000,
  B11111,B11111,B11111,B00000
};

byte eyeTopRight[8] = {
  B00000,B00000,B00000,B00000,
  B00000,B10000,B11000,B11110
};

// untere Reihe
byte eyeBottomLeftOuter[8] = {
  B00000,B00001,B00001,B00000,
  B00000,B00000,B00000,B00000
};

byte eyeBottomLeft[8] = {
  B11100,B11000,B11000,B00000,
  B00000,B00000,B00000,B00000
};

byte eyeBottomRight[8] = {
  B00111,B00011,B00011,B00000,
  B00000,B00000,B00000,B00000
};

byte eyeBottomRightOuter[8] = {
  B00000,B10000,B10000,B00000,
  B00000,B00000,B00000,B00000
};

void loadEyeChars() {
  lcd.createChar(0, eyeTopLeft);
  lcd.createChar(1, eyeTopMid);
  lcd.createChar(2, eyeTopRight);
  lcd.createChar(3, eyeBottomLeft);
  lcd.createChar(4, eyeBottomRight);
  lcd.createChar(5, eyeBottomLeftOuter);
  lcd.createChar(6, eyeBottomRightOuter);
}

// ================= AUGEN ZEICHNEN =================

void drawEyes() {

  // Linkes Auge
  lcd.setCursor(3, 0); lcd.write(byte(0));
  lcd.setCursor(4, 0); lcd.write(byte(1));
  lcd.setCursor(5, 0); lcd.write(byte(2));

  lcd.setCursor(2, 1); lcd.write(byte(5));
  lcd.setCursor(3, 1); lcd.write(byte(3));
  lcd.setCursor(5, 1); lcd.write(byte(4));
  lcd.setCursor(6, 1); lcd.write(byte(6));

  // Rechtes Auge (identisch)
  lcd.setCursor(10, 0); lcd.write(byte(0));
  lcd.setCursor(11, 0); lcd.write(byte(1));
  lcd.setCursor(12, 0); lcd.write(byte(2));

  lcd.setCursor(9, 1); lcd.write(byte(5));
  lcd.setCursor(10, 1); lcd.write(byte(3));
  lcd.setCursor(12, 1); lcd.write(byte(4));
  lcd.setCursor(13, 1); lcd.write(byte(6));
}

// ================= SERVO UPDATE =================

void UpdateServo() {
  if (millis() - lastServoUpdate >= servoInterval) {
    lastServoUpdate = millis();

    if (servoDirectionUp) {
      angle++;
      if (angle >= 180) servoDirectionUp = false;
    } else {
      angle--;
      if (angle <= 10) servoDirectionUp = true;
    }

    servo.write(angle);
  }
}
