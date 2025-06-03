#include "driver/ledc.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <Arduino.h>

// WiFi credentials
const char* ssid = "Sonar_Mies";
const char* password = "SonarMies2025";

// PWM configuration
const int pwm_frequency = 5000;
const int pwm_resolution = 8;

const int motorCount = 5;
const int motorPins[motorCount] = { 21, 17, 14, 26, 33 };
const int motorChannels[motorCount] = { 0, 1, 2, 3, 4 };

// OSC
WiFiUDP Udp;
const int localPort = 11111;

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi.");
    // Optional: Add fallback behavior
  }

  // Initialize PWM for each motor
  for (int i = 0; i < motorCount; i++) {
    ledcSetup(motorChannels[i], pwm_frequency, pwm_resolution);
    ledcAttachPin(motorPins[i], motorChannels[i]);
  }

  // Start listening for OSC
  Udp.begin(localPort);
  Serial.print("Listening for OSC on port ");
  Serial.println(localPort);
}

void handleMessage(OSCMessage& msg, String type) {
  if (msg.size() > 0 && msg.isFloat(0)) {
    float value = msg.getFloat(0);
    value = constrain(value, 0.0, 1.0);  // Clamp between 0 and 1
    int pwmValue = (int)(value * 255);

    Serial.print("Received ");
    Serial.print(type);
    Serial.print(" value: ");
    Serial.print(value);
    Serial.print(" -> PWM: ");
    Serial.println(pwmValue);

    if (type == "/hi") {
      ledcWrite(motorChannels[0], pwmValue);
    } else if (type == "/lo") {
      ledcWrite(motorChannels[1], pwmValue);
    } else if (type == "/mid") {
      ledcWrite(motorChannels[2], pwmValue);
    } else if (type == "/env") {
      ledcWrite(motorChannels[3], pwmValue);
    } else if (type == "/tri") {
      ledcWrite(motorChannels[4], pwmValue);
    }
  }
}

void loop() {
  OSCMessage msg;
  int packetSize = Udp.parsePacket();

  if (packetSize > 0) {
    while (packetSize--) {
      msg.fill(Udp.read());
    }

    if (!msg.hasError()) {
      char address[64];
      msg.getAddress(address, 0);

      if (strcmp(address, "/hi") == 0) {
        handleMessage(msg, "/hi");
      }
      if (strcmp(address, "/lo") == 0) {
        handleMessage(msg, "/lo");
      }
      if (strcmp(address, "/mid") == 0) {
        handleMessage(msg, "/mid");
      }
      if (strcmp(address, "/env") == 0) {
        handleMessage(msg, "/env");
      }
      if (strcmp(address, "/tri") == 0) {
        handleMessage(msg, "/tri");
      }
      if (strcmp(address, "/test") == 0) {
        handleMessage(msg, "/test");
      }
    }
  }

  delay(10);
}
