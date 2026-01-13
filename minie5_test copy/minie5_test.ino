// --- Arduino Code for 2 Microphones (A0, A1) --- //

const int NUM_MICS = 2;
const int micPins[NUM_MICS] = {A0, A1};

int baseline[NUM_MICS];
int smoothed[NUM_MICS];

void setup() {
  Serial.begin(115200);

  // calibrate
  delay(500);
  for (int m = 0; m < NUM_MICS; m++) {
    long sum = 0;
    for (int i = 0; i < 200; i++) {
      sum += analogRead(micPins[m]);
      delay(5);
    }
    baseline[m] = sum / 200;
    smoothed[m] = 0;
  }
}

void loop() {
  int loudness[NUM_MICS];

  for (int m = 0; m < NUM_MICS; m++) {
    int v = analogRead(micPins[m]);
    int delta = abs(v - baseline[m]);

    if (delta < 4) delta = 0;
    if (delta > 300) delta = 300;

    // scale 0–300 → 0–100
    int level = map(delta, 0, 300, 0, 100);

    // smoothing
    smoothed[m] = (smoothed[m] * 4 + level) / 5;
    loudness[m] = smoothed[m];
  }

  // send to Processing
  Serial.print(loudness[0]);
  Serial.print(",");
  Serial.println(loudness[1]);

  delay(10);
}

