#include "driver/ledc.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <Arduino.h>

// Red/Clave WiFi
const char* ssid = "Sonar_Mies";
const char* password = "SonarMies2025";

// Configurar PWM para motores
const int pwm_frequency = 5000;
const int pwm_resolution = 8;

const int motorCount = 3;
const int motorPins[motorCount] = { 17, 21 };
const int motorChannels[motorCount] = { 0, 1 };

// OSC
WiFiUDP Udp;
const int localPort = 11111;

int wifiConectado = false;

void setup() {
  Serial.begin(115200);

  // Conectar al WiFi
  Serial.println("Conectando a WiFi...");
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
    wifiConectado = true;
  } else {
    Serial.println("\nFailed to connect to WiFi.");
    wifiConectado = false;
  }

  // Inicializar motores
  for (int i = 0; i < motorCount; i++) {
    ledcSetup(motorChannels[i], pwm_frequency, pwm_resolution);
    ledcAttachPin(motorPins[i], motorChannels[i]);
  }

  // Comenzar a escuchar OSC
  Udp.begin(localPort);
  Serial.print("Esperando OSC en puerto ");
  Serial.println(localPort);
}

void loop() {
  if (wifiConectado) {
    handleMotorsOnNetwork();
  } else {
    handleMotorsNotOnNetwork();
  }
}

void handleMotorsOnNetwork() {
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
        handleMessage(msg, "/lo");
      }
    }
  }

  delay(10);
}

void handleMessage(OSCMessage& msg, String type) {
  if (msg.size() > 0 && msg.isFloat(0)) {
    float value = msg.getFloat(0);
    // value = constrain(value, 0.0, 1.0); // Limitar valores entre 0 y 1
    int pwmValue = (int)(value * 255);

    Serial.print("Received ");
    Serial.print(type);
    Serial.print(" value: ");
    Serial.print(value);
    Serial.print(" -> PWM: ");
    Serial.println(pwmValue);

    // if (type == "/hi") {
    //   ledcWrite(motorChannels[0], pwmValue);
    // }
    // if (type == "/lo") {
    //   ledcWrite(motorChannels[1], pwmValue);
    // }
    if (type == "/mid") {
      ledcWrite(motorChannels[0], pwmValue);
    }
    if (type == "/env") {
      ledcWrite(motorChannels[1], pwmValue);
    }
    // if (type == "/tri") {
    //   ledcWrite(motorChannels[0], pwmValue);
    // }
  }
}

void handleMotorsNotOnNetwork() {
  // AI prompt: help me define a pattern that imitates ambient music
  static unsigned long lastUpdate = 0;
  const int updateRate = 30;  // How often we update (in ms) for smooth transitions

  static float motorLevels[motorCount] = { 0 };         // Current PWM levels (0.0–255.0)
  static float targetLevels[motorCount] = { 0 };        // Target PWM levels
  static unsigned long nextChange[motorCount] = { 0 };  // When to change target again

  if (millis() - lastUpdate > updateRate) {
    lastUpdate = millis();

    for (int i = 0; i < motorCount; i++) {
      // Occasionally pick a new target (every 3–8 seconds)
      if (millis() > nextChange[i]) {
        targetLevels[i] = random(0, 2) == 0 ? random(30, 150) : 0;  // Soft pulses or fade out
        nextChange[i] = millis() + random(3000, 8000);
      }

      // Smoothly interpolate toward target
      float diff = targetLevels[i] - motorLevels[i];
      motorLevels[i] += diff * 0.02;  // Small smoothing factor

      // Clamp and write
      int pwmValue = constrain((int)motorLevels[i], 0, 255);
      ledcWrite(motorChannels[i], pwmValue);
    }
  }
}
