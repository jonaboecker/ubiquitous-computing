const int buttonPin = 2; 
const int ledPin = PIN_LED; 
int lastButtonState = HIGH; 
int ledState = LOW; 

void setup() {
  pinMode(ledPin, OUTPUT); 
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600); 
}

void loop() {
  int reading = digitalRead(buttonPin); // Den aktuellen Tasterzustand einlesen

  if (reading != lastButtonState) {
    if (reading == LOW) {
      Serial.println("1"); 
      digitalWrite(ledPin, HIGH); 
      ledState = HIGH;
    } else {
      Serial.println("0");  
      digitalWrite(ledPin, LOW); 
      ledState = LOW; 
    }
    lastButtonState = reading;
  } else{
  
  // Eingehende serielle Daten auslesen und die LED entsprechend steuern
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == '1' && ledState == LOW) {  
      digitalWrite(ledPin, HIGH); 
      Serial.println("1");
      ledState = HIGH; 
      reading = HIGH;
      lastButtonState = HIGH;
    } else if (command == '0' && ledState == HIGH) {  
      digitalWrite(ledPin, LOW);
      Serial.println("0");
      ledState = LOW;
      reading = LOW;
      lastButtonState = LOW;
    }; 
  }
  }
  delay(50);
}
