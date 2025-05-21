#include "driver/ledc.h"
#include <WiFi.h>
#include <Arduino.h>

const char* ssid = "Sonar_Mies";
const char* password = "SonarMies2025";

// PWM configuration constants
const int pwm_frequency = 5000;
const int pwm_resolution = 8;

const int motorCount = 5;

// Motor pins and channels
const int motorPins[motorCount] = {21, 17, 14, 26, 33};
const int motorChannels[motorCount] = {0, 1, 2, 3, 4};  // Use different LEDC channels for each motor

void setup() {
  Serial.begin(115200);
  delay(1000);

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
  }

  // Setup PWM for each motor
  for (int i = 0; i < motorCount; i++) {
    ledcSetup(motorChannels[i], pwm_frequency, pwm_resolution);
    ledcAttachPin(motorPins[i], motorChannels[i]);
  }
}

void loop() {
  // Ramp up PWM for all motors
  for (int ciclo = 0; ciclo <= 255; ciclo++) {
    for (int i = 0; i < motorCount; i++) {
      ledcWrite(motorChannels[i], ciclo);
    }
    delay(10);
  }

  // Ramp down PWM for all motors
  for (int ciclo = 255; ciclo >= 0; ciclo--) {
    for (int i = 0; i < motorCount; i++) {
      ledcWrite(motorChannels[i], ciclo);
    }
    delay(10);
  }
}
