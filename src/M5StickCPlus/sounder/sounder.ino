void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);

  tone(2, 440);
  delay(500);
  noTone(2);

}

bool flag;

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Hello\n");
  delay(2500);

  Serial.print("Tone\n");
  tone(2, 140);
  delay(4500);
  noTone(2);
  Serial.print("LED\n");

  if ( flag == true ) {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }
  flag = !flag;
}
