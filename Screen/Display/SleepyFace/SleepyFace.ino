#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================== TILES ==================

// y = 0 (oben)
byte char30[8] = { B00000, B00000, B00000, B00000, B00000, B00001, B00011, B01111 };
byte char40[8] = { B00000, B00000, B00000, B00000, B11111, B11111, B11111, B00000 };
byte char50[8] = { B00000, B00000, B00000, B00000, B00000, B10000, B11000, B11110 };

byte char100[8] = { B00000, B00000, B00000, B00000, B00000, B00001, B00011, B01111 };
byte char110[8] = { B00000, B00000, B00000, B00000, B11111, B11111, B11111, B00000 };
byte char120[8] = { B00000, B00000, B00000, B00000, B00000, B10000, B11000, B11110 };

// y = 1 (unten)
byte char21[8] = { B00000, B00001, B00001, B00000, B00000, B00000, B00000, B00000 };
byte char31[8] = { B11100, B11000, B11000, B00000, B00000, B00000, B00000, B00000 };
byte char51[8] = { B00111, B00011, B00011, B00000, B00000, B00000, B00000, B00000 };
byte char61[8] = { B00000, B10000, B10000, B00000, B00000, B00000, B00000, B00000 };

byte char121[8] = { B00111, B00011, B00011, B00000, B00000, B00000, B00000, B00000 };
byte char131[8] = { B00000, B10000, B10000, B00000, B00000, B00000, B00000, B00000 };

byte char91[8] = { B00000, B00001, B00001, B00000, B00000, B00000, B00000, B00000 };
byte char101[8] = { B11100, B11000, B11000, B00000, B00000, B00000, B00000, B00000 };

// ================== SET A ==================
// obere Hälfte
void loadSetA() {
  lcd.createChar(0, char30);   // x3 y0
  lcd.createChar(1, char40);   // x4 y0
  lcd.createChar(2, char50);   // x5 y0
  lcd.createChar(3, char100);  // x10 y0
  lcd.createChar(4, char110);  // x11 y0
  lcd.createChar(5, char120);  // x12 y0
  lcd.createChar(6, char21);   // x2 y1
  lcd.createChar(7, char31);   // x3 y1
}

// ================== SET B ==================
// untere Hälfte
void loadSetB() {
  lcd.createChar(0, char51);   // x5 y1
  lcd.createChar(1, char61);   // x6 y1
  lcd.createChar(2, char121);  // x12 y1
  lcd.createChar(3, char131);  // x13 y1
  lcd.createChar(4, char91);   // x9 y1
  lcd.createChar(5, char101);  // x10 y1
  lcd.createChar(6, char30);   // dummy
  lcd.createChar(7, char30);   // dummy
}

// ================== DRAW ==================
void drawSetA() {
  lcd.setCursor(3, 0);
  lcd.write(byte(0));
  lcd.setCursor(4, 0);
  lcd.write(byte(1));
  lcd.setCursor(5, 0);
  lcd.write(byte(2));
  lcd.setCursor(10, 0);
  lcd.write(byte(3));
  lcd.setCursor(11, 0);
  lcd.write(byte(4));
  lcd.setCursor(12, 0);
  lcd.write(byte(5));

  lcd.setCursor(2, 1);
  lcd.write(byte(6));
  lcd.setCursor(3, 1);
  lcd.write(byte(7));
}

void drawSetB() {
  lcd.setCursor(5, 1);
  lcd.write(byte(0));
  lcd.setCursor(6, 1);
  lcd.write(byte(1));
  lcd.setCursor(12, 1);
  lcd.write(byte(2));
  lcd.setCursor(13, 1);
  lcd.write(byte(3));
  lcd.setCursor(9, 1);
  lcd.write(byte(4));
  lcd.setCursor(10, 1);
  lcd.write(byte(5));
}

// ================== SETUP / LOOP ==================
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  loadSetA();
  drawSetA();
  delayMicroseconds(15000);
  lcd.clear();

  loadSetB();
  drawSetB();
  delayMicroseconds(15000);
  lcd.clear();
}
