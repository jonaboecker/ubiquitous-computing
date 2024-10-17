#include <WiFiNINA.h>
#include <Arduino_LSM6DSOX.h>

void setup() {
  pinMode(LEDG, OUTPUT); //define GREEN LED
  pinMode(LEDR, OUTPUT); //define RED LED
  pinMode(LEDB, OUTPUT); //define BLUE LED
  Serial.begin(9600);
}

void loop() {
  int temp_deg = 0;
  IMU.readTemperature(temp_deg);
  if (temp_deg > 32)
    setLedRed();
  else if (temp_deg < 25)
    setLedBlue();
  else
    setLedGreen();
  Serial.print("Temperatur: ");
  delay(1000);
}

void setLedBlue() {
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, HIGH);
}

void setLedRed() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
}

void setLedGreen() {
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, LOW);
}
