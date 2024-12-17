#include <WiFiNINA.h>

const int ledPin = 13;

const char* ssid = "My_WiFi_SSID";
const char* password = "My_WiFi password could be here";

WiFiServer server(80);

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi module not found! Check hardware.");
    while (true);
  }

  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("HTTP server started.");
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client connected.");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><title>LED Control</title></head>");
            client.println("<body>");
            client.println("<h1>Arduino Nano RP2040 LED Control</h1>");
            client.println("<p>Click below to control the LED:</p>");
            client.println("<a href=\"/1\"><button>Turn LED ON</button></a>");
            client.println("<a href=\"/0\"><button>Turn LED OFF</button></a>");
            client.println("</body></html>");

            break;
          } else {
            if (currentLine.indexOf("GET /1") >= 0) {
              digitalWrite(ledPin, HIGH);
              Serial.println("LED ON via Web");
            }
            if (currentLine.indexOf("GET /0") >= 0) {
              digitalWrite(ledPin, LOW);
              Serial.println("LED OFF via Web");
            }
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    client.stop();
    Serial.println("Client disconnected.");
  }
}
