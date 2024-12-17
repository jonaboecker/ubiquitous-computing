#include <WiFiNINA.h>
#include <Arduino_LSM6DSOX.h>

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  delay(5000);
  if(!IMU.begin()) {
    Serial.println("IMU setup failed");
  }
}

void loop() {
  int temp_deg = 0;
  IMU.readTemperature(temp_deg);
  temp_deg -= 12;
  Serial.println(temp_deg);
  delay(1000);
}
