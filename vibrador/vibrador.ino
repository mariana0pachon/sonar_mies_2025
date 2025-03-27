int vibratorPin = 5;
const int maxBeats = 7;
int patternBeats;
int intensities[maxBeats];
int durations[maxBeats];
int pauses[maxBeats];

void setup() {
  pinMode(vibratorPin, OUTPUT);
  randomSeed(analogRead(A0));

  patternBeats = random(3, maxBeats + 1);

  for (int i = 0; i < patternBeats; i++) {
    intensities[i] = random(100, 256);
    durations[i] = random(50, 300);
    pauses[i] = random(100, 400);
  }
}

void loop() {
  for (int i = 0; i < patternBeats; i++) {
    analogWrite(vibratorPin, intensities[i]);
    delay(durations[i]);
    analogWrite(vibratorPin, 0);
    delay(pauses[i]);
  }
}
