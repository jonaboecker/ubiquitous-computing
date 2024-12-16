// Define pin numbers
const int ledPin = 13;      // LED connected to digital pin 13
const int buttonPin = 2;    // Push button connected to digital pin 2

void setup() {
  pinMode(ledPin, OUTPUT);     // Set LED pin as output
  pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with pull-up resistor
  Serial.begin(9600);          // Start serial communication at 9600 baud rate
}

void loop() {
  // Read the button state
  int buttonState = digitalRead(buttonPin);
  
  // Send button state via Serial
  if (buttonState == LOW) {    // Button pressed
    Serial.println("1");
  } else {                     // Button not pressed
    Serial.println("0");
  }

  // Check for incoming Serial data
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the received character
    if (command == '1') {
      digitalWrite(ledPin, HIGH); // Turn LED on
    } else if (command == '0') {
      digitalWrite(ledPin, LOW);  // Turn LED off
    }
  }

  delay(100); // Short delay for debounce
}
