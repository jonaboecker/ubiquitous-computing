#include <Arduino_LSM6DSOX.h>
#include <MadgwickAHRS.h>
#include <WiFiNINA.h>

Madgwick filter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;

void setup() {
  Serial.begin(9600);
  pinMode(LEDB, OUTPUT);

  // start the IMU and filter
  if (!IMU.begin()) {
      Serial.println("Failed to initialize IMU!");
      while (1);
  }
  filter.begin(104);
}

void loop() {
  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;

  int temp_deg = 0;
  IMU.readTemperature(temp_deg);
  temp_deg -= 12;

  // read raw data from IMU
  IMU.readAcceleration(ax, ay, az);
  IMU.readGyroscope(gx, gy, gz);

  // update the filter, which computes orientation
  filter.updateIMU(gx, gy, gz, ax, ay, az);

  // print the heading, pitch and roll
  //roll = filter.getRoll();
  pitch = filter.getPitch();
  //heading = filter.getYaw();
  Serial.print("Orientation: ");
  //Serial.print(heading);
  //Serial.print(" ");
  Serial.print(pitch);

  Serial.print("\t Temperatur: ");
  Serial.print(temp_deg);
  Serial.print("\n");

  //Serial.print(" ");
  //Serial.println(roll);

  if (pitch < 80 || temp_deg > 26 || temp_deg < 18) {
    digitalWrite(LEDB, HIGH);
  } else {
    digitalWrite(LEDB, LOW);
  }

  delay(5);

}