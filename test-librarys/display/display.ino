/*
Nur Display mit Augen (Arduino Uno + I2C LCD)
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ================= DISPLAY =================

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= SETUP =================

void setup() {

  lcd.init();
  lcd.backlight();

  loadEyeChars();
  drawEyes();
}

// ================= LOOP =================

void loop() {
  // nichts nötig (statisches Display)
}

// ================= AUGE CUSTOM CHARS =================

byte eyeTopLeft[8] = {
  B00000, B00000, B00000, B00000,
  B00000, B00001, B00011, B01111
};

byte eyeTopMid[8] = {
  B00000, B00000, B00000, B00000,
  B11111, B11111, B11111, B00000
};

byte eyeTopRight[8] = {
  B00000, B00000, B00000, B00000,
  B00000, B10000, B11000, B11110
};

byte eyeBottomLeft[8] = {
  B11100, B11000, B11000, B00000,
  B00000, B00000, B00000, B00000
};

byte eyeBottomRight[8] = {
  B00111, B00011, B00011, B00000,
  B00000, B00000, B00000, B00000
};

byte eyeBottomLeftOuter[8] = {
  B00000, B00001, B00001, B00000,
  B00000, B00000, B00000, B00000
};

byte eyeBottomRightOuter[8] = {
  B00000, B10000, B10000, B00000,
  B00000, B00000, B00000, B00000
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
  lcd.setCursor(3, 0);
  lcd.write(byte(0));
  lcd.setCursor(4, 0);
  lcd.write(byte(1));
  lcd.setCursor(5, 0);
  lcd.write(byte(2));

  lcd.setCursor(2, 1);
  lcd.write(byte(5));
  lcd.setCursor(3, 1);
  lcd.write(byte(3));
  lcd.setCursor(5, 1);
  lcd.write(byte(4));
  lcd.setCursor(6, 1);
  lcd.write(byte(6));

  // Rechtes Auge
  lcd.setCursor(10, 0);
  lcd.write(byte(0));
  lcd.setCursor(11, 0);
  lcd.write(byte(1));
  lcd.write(byte(1));
  lcd.setCursor(12, 0);
  lcd.write(byte(2));

  lcd.setCursor(9, 1);
  lcd.write(byte(5));
  lcd.setCursor(10, 1);
  lcd.write(byte(3));
  lcd.setCursor(12, 1);
  lcd.write(byte(4));
  lcd.setCursor(13, 1);
  lcd.write(byte(6));
}
