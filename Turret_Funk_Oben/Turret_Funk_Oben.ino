
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
/*
Empfänger mit:
- Display (Augen)
- DC Motoren
- Servo (Pfeil)
- NRF24L01 Empfang von Y + Fire
*/

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// ================= NRF24 =================

RF24 radio(9, 10); // CE, CSN
const byte addressY[6] = "00002";

struct YFireData {
  int16_t y;
  uint8_t fire;   // statt bool
};

YFireData receivedData;

// ================= DISPLAY =================

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= SERVO =================

Servo servo;

const int SERVO_REST = 10;      // Ausgangsposition
const int SERVO_FIRE = 120;     // Schuss Position
bool isFiring = false;
bool lastFireState = false;

// ================= SETUP =================

void setup() {

  Serial.begin(9600);

  InitializeDCMotor();

  // Servo
  servo.attach(8);
  servo.write(SERVO_REST);

  // Display
  lcd.init();
  lcd.backlight();
  loadEyeChars();
  drawEyes();

  // Radio
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, addressY);
  radio.startListening();
}

// ================= LOOP =================

void loop() {

  if (radio.available()) {

    radio.read(&receivedData, sizeof(receivedData));

    Serial.print("Empfangen -> Y: ");
    Serial.print(receivedData.y);
    Serial.print(" | Fire: ");
    Serial.println(receivedData.fire ? "JA" : "NEIN");

    bool currentFire = (receivedData.fire == 1);

    // Nur auslösen wenn Knopf neu gedrückt wurde
    if (currentFire && !lastFireState && !isFiring) {
      Serial.println(">>> FIRE TRIGGERED <<<");
      fireArrow();
    }

    lastFireState = currentFire;

  } else {
    Serial.println("Keine Daten empfangen...");
  }

  delay(100);  // damit es lesbar bleibt
}



// ================= FIRE FUNKTION =================

void fireArrow() {

  isFiring = true;

  Serial.println("FIRE!");

  // Vorfahren
  servo.write(SERVO_FIRE);
  delay(300);   // Zeit zum Pfeil schieben

  // Zurückfahren
  servo.write(SERVO_REST);
  delay(300);   // Zeit zum Zurückfahren

  isFiring = false;
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

byte eyeBottomLeft[8] = {
  B11100,B11000,B11000,B00000,
  B00000,B00000,B00000,B00000
};

byte eyeBottomRight[8] = {
  B00111,B00011,B00011,B00000,
  B00000,B00000,B00000,B00000
};

byte eyeBottomLeftOuter[8] = {
  B00000,B00001,B00001,B00000,
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

  // Rechtes Auge
  lcd.setCursor(10, 0); lcd.write(byte(0));
  lcd.setCursor(11, 0); lcd.write(byte(1));
  lcd.setCursor(12, 0); lcd.write(byte(2));

  lcd.setCursor(9, 1); lcd.write(byte(5));
  lcd.setCursor(10, 1); lcd.write(byte(3));
  lcd.setCursor(12, 1); lcd.write(byte(4));
  lcd.setCursor(13, 1); lcd.write(byte(6));
}
