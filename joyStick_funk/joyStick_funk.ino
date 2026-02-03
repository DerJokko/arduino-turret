#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

const int VRx_PIN = A0;
const int DEADZONE = 20;

int16_t sendValue = 0;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

void loop() {
  int raw = analogRead(VRx_PIN);   // 0–1023
  int centered = raw - 512;        // -512 … +511

  // Deadzone → 0 senden
  if (abs(centered) < DEADZONE) {
    sendValue = 0;
  } else {
    sendValue = centered;
  }

  radio.write(&sendValue, sizeof(sendValue));

  Serial.print("Send: ");
  Serial.println(sendValue);

  delay(20);
}
