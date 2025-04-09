int vibratorPin1 = 5;
int vibratorPin2 = 6;
int ledPin1 = 7;
int ledPin2 = 8;

void setup() {
  pinMode(vibratorPin1, OUTPUT);
  pinMode(vibratorPin2, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  randomSeed(analogRead(A0)); // Random "simulated beat input"
}

void loop() {
  // Simulate a beat
  int simulatedVolume = random(100, 256);     // Map from music volume
  int duration = random(50, 150);             // Simulate beat length
  int pause = random(100, 300);               // Pause between beats

  // Alternate vibrators
  static bool toggle = false;

  if (toggle) {
    analogWrite(vibratorPin1, simulatedVolume);
    digitalWrite(ledPin1, HIGH);
    delay(duration);
    analogWrite(vibratorPin1, 0);
    digitalWrite(ledPin1, LOW);
  } else {
    analogWrite(vibratorPin2, simulatedVolume);
    digitalWrite(ledPin2, HIGH);
    delay(duration);
    analogWrite(vibratorPin2, 0);
    digitalWrite(ledPin2, LOW);
  }

  toggle = !toggle; // alternate for next beat
  delay(pause);
}
