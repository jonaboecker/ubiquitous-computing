#include <WiFiNINA.h>

const char *ssid = "Your_SSID";           // Wi-Fi network name for client mode
const char *password = "Your_PASSWORD";   // Wi-Fi password for client mode
const char *ap_ssid = "Nano_AP";          // Name of the network created in AP mode

WiFiServer server(80); // Create a server on port 80

void setup() {
  Serial.begin(9600);
  
  // Initialize LEDs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // Attempt to connect as a client
  if (connectToWiFi()) {
    Serial.println("Connected to WiFi. Starting server...");
    server.begin();
  } else {
    // If client connection fails, start AP mode
    setupAccessPoint();
  }
}

void loop() {
  // Check for client connections in either mode
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected!");
    String request = client.readStringUntil('\r');
    client.flush();

    // Handle LED control based on request
    if (request.indexOf("/LEDON") != -1) {
      setLEDState(true);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      client.println("<html><body><h1>LED is ON</h1></body></html>");
    } 
    else if (request.indexOf("/LEDOFF") != -1) {
      setLEDState(false);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      client.println("<html><body><h1>LED is OFF</h1></body></html>");
    }
    else if (request.indexOf("/RGB") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      client.println("<html><body>");
      client.println("<h1>Control RGB LED</h1>");
      client.println("<p><a href=\"/LEDON\">Turn LED ON</a></p>");
      client.println("<p><a href=\"/LEDOFF\">Turn LED OFF</a></p>");
      client.println("</body></html>");
    }
    client.stop();
    Serial.println("Client disconnected.");
  }
}

// Function to connect to Wi-Fi in client mode
bool connectToWiFi() {
  Serial.print("Attempting to connect to network: ");
  Serial.println(ssid);
  
  int status = WiFi.begin(ssid, password);
  unsigned long startAttemptTime = millis();

  // Retry for up to 10 seconds
  while (status != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
    status = WiFi.status();
  }
  Serial.println();

  if (status == WL_CONNECTED) {
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("Failed to connect as a client.");
    return false;
  }
}

// Function to set up the device as an Access Point
void setupAccessPoint() {
  Serial.print("Setting up Access Point: ");
  Serial.println(ap_ssid);

  if (WiFi.beginAP(ap_ssid)) {
    Serial.println("Access Point started successfully.");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
  } else {
    Serial.println("Failed to start Access Point.");
  }
}

// Function to control the onboard RGB LED
void setLEDState(bool on) {
  if (on) {
    digitalWrite(LEDR, HIGH);  // Turn on red for simplicity
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
    Serial.println("LED is ON");
  } else {
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
    Serial.println("LED is OFF");
  }
}
