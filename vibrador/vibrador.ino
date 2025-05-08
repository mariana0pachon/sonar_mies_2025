#include <WiFi.h>
#include <Arduino.h>

const char* ssid = "Sonar_Mies";
const char* password = "SonarMies2025";

//variables para pin 21 (conectado a IN1 en driver) - control MOTOR A
const int in1_Channel = 0;
const int in1_Pin = 21;
const int in1_frequency = 5000;
const int in1_resolution = 8;

//variables para pin x (conectado a IN3 en driver) - control MOTOR B
const int in3_Channel = 1;  // creo que no hay que repetir numero de canal por eso pongo 1
const int in3_Pin = 0;      //revisar cual es el pin conectado a IN3
const int in3_frequency = 5000;
const int in3_resolution = 8;

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

  //declarar pin como pwm
  ledcSetup(in1_Channel, in1_frequency, in1_resolution);
  ledcAttachPin(in1_Pin, in1_Channel);

  //declarar pin como pwm
  ledcSetup(in3_Channel, in3_frequency, in3_resolution);
  ledcAttachPin(in3_Pin, in3_Channel);
}

void loop() {
  //ciclo para subir y bajar la potencia del motor
  for (int ciclo = 0; ciclo <= 255; ciclo++) {
    ledcWrite(in1_Channel, ciclo);  //esto funciona como analogWrite
    //ledcWrite(in3_Channel, ciclo);
    delay(10);
  }

  for (int ciclo = 255; ciclo >= 0; ciclo--) {
    ledcWrite(in1_Channel, ciclo);  //motor A
    //ledcWrite(in3_Channel, ciclo); //motor B
    delay(10);
  }
}
