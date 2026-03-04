#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// I2C-Adresse ggf. anpassen: 0x27 oder 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();       // LCD starten
  lcd.backlight();  // Licht an
}

void loop() {

  // 🔹 1. Einfacher Text
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hallo!");
  lcd.setCursor(0, 1);
  lcd.print("LCD Demo");
  delay(2000);

  // 🔹 2. Zahlen anzeigen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Zahl:");
  lcd.setCursor(6, 0);
  lcd.print(1234);
  delay(2000);

  // 🔹 3. Cursor bewegen
  lcd.clear();
  lcd.print("Cursor Test");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("^ hier");
  delay(2000);

  // 🔹 4. Bildschirm loeschen
  lcd.clear();
  lcd.print("Clear()");
  delay(1500);

  // 🔹 5. Scrollender Text
  lcd.clear();
  lcd.print("Scrollender Text");
  for (int i = 0; i < 16; i++) {
    lcd.scrollDisplayLeft();
    delay(300);
  }
  delay(1000);

  // 🔹 6. Hintergrundlicht aus/an
  lcd.clear();
  lcd.print("Backlight aus");
  delay(1000);
  lcd.noBacklight();
  delay(1000);
  lcd.backlight();

  // 🔹 7. millis() anzeigen (Zeit)
  lcd.clear();
  lcd.print("Millis:");
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
  delay(2000);
}
