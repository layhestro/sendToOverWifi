#include <SPI.h>
#include <WiFiNINA.h>
#include <Wire.h>
// Include your temperature sensor's library here if necessary

// Replace with your network credentials
const char* ssid = "your_network_SSID";
const char* password = "your_network_password";

// Server details
const char* server = "your.server.com";
const int httpPort = 80; // or use the appropriate port for your server

// Initialize the WiFi client library
WiFiClient client;

// Define an array or data structure to hold the temperature readings and timestamps
struct TemperatureReading {
  String timestamp;
  float temperature;
};

// Define an array to hold the temperature readings
const int readingsPerPeriod = 240; // 1 reading every 5 seconds for 10 minutes
float temperatureReadings[readingsPerPeriod];
unsigned long lastReadingTime = 0; // track when the last reading was taken
const long readingInterval = 5000; // 5 seconds in milliseconds

unsigned long lastSendTime = 0; // track when data was last sent
const long sendInterval = 600000; // 10 minutes in milliseconds

void setup() {
  Serial.begin(9600);
  // Initialize WiFi
  connectToWifi();
  // Initialize temperature sensor
  // No initialization necessary for analog sensors like TMP36
}

void loop() {
  takeTemperatureReading();
  trySendingData();
}

void takeTemperatureReading() {
  if (millis() - lastReadingTime >= readingInterval) {
    // Read the temperature from the sensor
    temperatureReadings[(millis() - lastSendTime) / readingInterval % readingsPerPeriod] = readTemperatureCelsius();
    lastReadingTime = millis();
  }
}

void trySendingData() {
  // Check if it's time to send the data
  if (millis() - lastSendTime >= sendInterval) {
    // Connect to WiFi
    connectToWifi();
    
    // Check if WiFi connection was successful
    if (WiFi.status() == WL_CONNECTED) {
      sendTemperatureReadings();
      // Disconnect from WiFi after sending data
      WiFi.disconnect();
    }
    
    // Reset the last send time
    lastSendTime = millis();
  }
}

void connectToWifi() {
  // Attempt to connect to WiFi network
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  // Wait for connection result with a timeout
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 30000) { // 30 seconds timeout
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  } else {
    Serial.println("Failed to connect to WiFi");
  }
}

float readTemperatureCelsius() {
  // Replace this with the actual reading code from your sensor
  return 0.0;
}

void sendTemperatureReadings() {
  if (WiFi.status() == WL_CONNECTED && client.connect(server, httpPort)) {
    Serial.println("Connected to server");
    String jsonPayload = createJsonPayload();
    postPayload(jsonPayload);
    client.stop();
  } else {
    Serial.println("Failed to connect to server");
  }
}

String createJsonPayload() {
  String jsonPayload = "{\"temperatureReadings\":[";
  for (int i = 0; i < readingsPerPeriod; i++) {
    jsonPayload += String(temperatureReadings[i]);
    if (i < readingsPerPeriod - 1) jsonPayload += ",";
  }
  jsonPayload += "]}";
  return jsonPayload;
}

void postPayload(String jsonPayload) {
  // Start a POST request
  client.println("POST /data HTTP/1.1");
  client.println("Host: " + String(server));
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonPayload.length());
  client.println("Connection: close");
  client.println();
  client.println(jsonPayload);

  // End the POST request
  client.println();

  // Wait for the response from the server
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  }
}
