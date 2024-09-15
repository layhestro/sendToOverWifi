# Send Temperature Data Over WiFi

## Overview

This project demonstrates how to read temperature data from a sensor and send it over WiFi to a server. The program collects temperature readings every 5 seconds, stores them in an array, and sends the data to a server every 10 minutes. The communication is handled using the `WiFiNINA` library.

## Features

- **WiFi Connection**: The program connects to a specified WiFi network and sends data to a remote server.
- **Temperature Readings**: The program collects temperature data from a sensor every 5 seconds.
- **Data Transmission**: After collecting data for 10 minutes, the program sends the readings as a JSON payload to a server via a POST request.
- **Error Handling**: Basic error handling for WiFi connection failures and unsuccessful server communication.

## Requirements

- **Arduino Board**: Compatible with `WiFiNINA` library (e.g., Arduino MKR WiFi 1010, Arduino Nano 33 IoT).
- **WiFiNINA Library**: The `WiFiNINA` library is used to handle WiFi communication.
- **Temperature Sensor**: Any temperature sensor can be used (e.g., TMP36), though the current code includes a placeholder function for reading temperature data.

## Code Breakdown

- **WiFi Setup**: Connects to the WiFi network using provided credentials:
  const char* ssid = "your_network_SSID";
  const char* password = "your_network_password";
- **Temperature Data Collection: Readings are taken every 5 seconds:**
  const long readingInterval = 5000; // 5 seconds in milliseconds
- **Data Transmission: After 10 minutes, the temperature readings are sent to the server::**
  const long sendInterval = 600000; // 10 minutes in milliseconds
- **Server Communication: Data is sent via HTTP POST request as a JSON payload to the specified server.**



## How to Use

Configure WiFi: Update the WiFi credentials with your network's SSID and password.
const char* ssid = "your_network_SSID";
const char* password = "your_network_password";

Set up the Server: Update the server address and port information.
const char* server = "your.server.com";
const int httpPort = 80;
Compile and Upload: Use the Arduino IDE to compile and upload the code to your Arduino board.
Monitor Serial Output: Open the serial monitor to see the connection status, temperature readings, and server response.
