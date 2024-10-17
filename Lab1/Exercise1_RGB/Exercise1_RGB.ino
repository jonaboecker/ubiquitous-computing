#include <WiFiNINA.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDG, OUTPUT); //define GREEN LED
  pinMode(LEDR, OUTPUT); //define RED LED
  pinMode(LEDB, OUTPUT); //define BLUE LED
}

void loop() {
  // RED
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
  delay(500);
  // GREEN
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDR, LOW);
  delay(500);
  // BLUE
  digitalWrite(LEDB, HIGH);
  digitalWrite(LEDG, LOW);
  delay(500);
  // GREEN
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, LOW);
  delay(500);
}
