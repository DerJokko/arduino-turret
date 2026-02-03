#include <Stepper.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// ---------- Funk ----------
RF24 radio(9, 10);
const byte address[6] = "00001";

struct DataPacket {
  int vrx;
  int vry;
  bool fire;
};

DataPacket data;

// ---------- Stepper ----------
const int stepsPerRevolution = 2048;
Stepper motorHorizontal(stepsPerRevolution, 8, 10, 9, 11);

// --- Konfiguration ---
int idleValue = 512;
int maxRPM = 16;
int puffer = 20;

int lastVRx = 512;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(data));
    lastVRx = data.vrx;

    if (data.fire) {
      Serial.println("Fire!");
    }
  }

  MoveHorizontal(lastVRx);
  delay(20);
}

void MoveHorizontal(int joystickValue) {
  int delta = joystickValue - idleValue;

  if (abs(delta) < puffer) return;

  float speedFraction = (float)delta / 512.0;
  int motorRPM = speedFraction * maxRPM;

  int direction = (motorRPM > 0) ? 1 : -1;

  const int stepMultiplier = 4;
  int stepsToMove = abs(motorRPM) * stepMultiplier;

  motorHorizontal.setSpeed(abs(motorRPM));
  motorHorizontal.step(stepsToMove * direction);

  Serial.print("VRx: ");
  Serial.print(joystickValue);
  Serial.print("  delta: ");
  Serial.print(delta);
  Serial.print("  RPM: ");
  Serial.println(motorRPM);
}
