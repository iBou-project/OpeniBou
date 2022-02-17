#include "M5StickCPlus.h"

void setup() {
  M5.begin();

  pinMode(33, OUTPUT);
  
}

void loop() {
  digitalWrite(33, HIGH);
  delay(1000);
  digitalWrite(33, LOW);
  delay(1000);
}
