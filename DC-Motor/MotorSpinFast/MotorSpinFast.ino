int MA1 = 9; // Pin 9 kann 
int MA2 = 8;

void setup() {
  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);

  digitalWrite(in2MA2, LOW);   // Richtung festlegen
}

void loop() {
  analogWrite(MA1, 255);    // volle Drehzahl
}