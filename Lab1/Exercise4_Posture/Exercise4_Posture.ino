#include <Arduino_LSM6DSOX.h>
#include <MadgwickAHRS.h>

Madgwick filter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;

void setup() {
  Serial.begin(9600);

  // start the IMU and filter
  if (!IMU.begin()) {
      Serial.println("Failed to initialize IMU!");
      while (1);
  }
  filter.begin(25);
}

void loop() {
  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;

  // read raw data from IMU
  IMU.readAcceleration(ax, ay, az);
  IMU.readGyroscope(gx, gy, gz);

  // update the filter, which computes orientation
  filter.updateIMU(gx, gy, gz, ax, ay, az);

  // print the heading, pitch and roll
  roll = filter.getRoll();
  pitch = filter.getPitch();
  heading = filter.getYaw();
  Serial.print("Orientation: ");
  Serial.print(heading);
  Serial.print(" ");
  Serial.print(pitch);
  Serial.print(" ");
  Serial.println(roll);
  
}