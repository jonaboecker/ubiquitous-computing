#include <WiFiNINA.h>
#include <PDM.h>
#include <Arduino_LSM6DSOX.h>
#include <MadgwickAHRS.h>

const int noiseThreshold = 10000;   // Threshold for sound level

Madgwick madgwickFilter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;

short sampleBuffer[2048];
volatile int samplesRead;
bool postureAlarm = false;
bool noiseAlarm = false;
bool prevAlarm = false;
unsigned long alarmEndTime = 0;

void setup() {
  pinMode(LEDG, OUTPUT); //define GREEN LED
  pinMode(LEDR, OUTPUT); //define RED LED
  pinMode(LEDB, OUTPUT); //define BLUE LED

  setLedGreen();

  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  // Initialize the microphone
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }
  PDM.onReceive(onPDMdata);

  // start the IMU and filter
  if (!IMU.begin()) {
      Serial.println("Failed to initialize IMU!");
      while (1);
  }
  madgwickFilter.begin(104);
}

void loop() {
  if (samplesRead) {
    // Check if noise level is above threshold
    for (int i = 0; i < samplesRead; i++) {
      if (sampleBuffer[i] > noiseThreshold || sampleBuffer[i] < -noiseThreshold) {
        noiseAlarm = true;
        break;
      } else {
        noiseAlarm = false;
      }
    }
    samplesRead = 0; // Clear sample read count
  }

  // Check posture using accelerometer data
  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;

  // read raw data from IMU
  IMU.readAcceleration(ax, ay, az);
  IMU.readGyroscope(gx, gy, gz);

  // update the filter, which computes orientation
  madgwickFilter.updateIMU(gx, gy, gz, ax, ay, az);

  pitch = madgwickFilter.getPitch();
  Serial.print("Orientation: ");
  Serial.println(pitch);

  if (pitch < 1) {
    postureAlarm = true;
  } else {
    postureAlarm = false;
  }

  // Control LED outputs based on alarms
  if (noiseAlarm) {
    blinkLEDRed();
    setPrevAlarm();
  } else if (postureAlarm) {
    blinkLEDBlue();
    setPrevAlarm();
  } else if (prevAlarm == true && millis() < alarmEndTime) {
    alarmEndTime = 0;
    prevAlarm == false;
    setLedGreen();
  } else {
    setLedGreen();
  }

  delay(500);  // Blink interval delay
}

void setPrevAlarm() {
  prevAlarm = millis() + 10000;
  prevAlarm == true;
}

void blinkLEDBlue() {
  setLedBlue();
  delay(250);
  digitalWrite(LEDB, LOW);
  delay(250);
}

void blinkLEDRed() {
  setLedRed();
  delay(250);
  digitalWrite(LEDR, LOW);
  delay(250);
}

// Callback function for PDM microphone data
void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2; // 16-bit samples
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
