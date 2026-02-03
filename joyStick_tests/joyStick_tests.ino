int FIRE_PIN = 2;
int VRx= 0;
int VRy = 1;


void setup() {
  // put your setup code here, to run once:
  pinMode(FIRE_PIN, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(FIRE_PIN)==HIGH) {
    Serial.println("Fire");
  }

  VRx = analogRead(A0);  // liest 0–1023
  VRy = analogRead(A1);  // liest 0–1023
  Serial.print("VRx: ");
  Serial.print(VRx);
  Serial.print(", VRy: ");
  Serial.println(VRy);
  delay(100);
}
