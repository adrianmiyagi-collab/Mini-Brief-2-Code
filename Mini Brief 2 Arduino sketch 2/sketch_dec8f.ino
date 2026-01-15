const int NUM_MICS = 2;                       // ahora solo 2 micrófonos
const int micPins[NUM_MICS] = {A0, A1};

int baseline[NUM_MICS];   // baseline para cada mic

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUM_MICS; i++) {
    pinMode(micPins[i], INPUT);
  }

  // ==== Calibrar "silencio" en cada mic ====
  const int samples = 200;
  delay(500);  // tiempo para estabilizar

  for (int m = 0; m < NUM_MICS; m++) {
    long sum = 0;
    for (int i = 0; i < samples; i++) {
      int v = analogRead(micPins[m]);
      sum += v;
      delay(5);
    }
    baseline[m] = sum / samples;

    Serial.print("Baseline mic ");
    Serial.print(m);
    Serial.print(" (pin ");
    Serial.print(micPins[m]);
    Serial.print("): ");
    Serial.println(baseline[m]);
  }
}

void loop() {
  const int samples = 80;
  int maxDelta[NUM_MICS] = {0, 0};   // solo 2 micros

  // buscamos el mayor cambio respecto al baseline para cada mic
  for (int i = 0; i < samples; i++) {
    for (int m = 0; m < NUM_MICS; m++) {
      int v = analogRead(micPins[m]);
      int delta = abs(v - baseline[m]);
      if (delta > maxDelta[m]) {
        maxDelta[m] = delta;
      }
    }
    delayMicroseconds(200);
  }

  // ajuste lento de baseline cuando hay poco ruido
  for (int m = 0; m < NUM_MICS; m++) {
    if (maxDelta[m] < 3) {                  // casi silencio
      int v = analogRead(micPins[m]);
      baseline[m] = (baseline[m] * 99 + v) / 100;  // filtro suave
    }
  }

  // loudness para cada mic 512–1023
  int loudness[NUM_MICS];
  for (int m = 0; m < NUM_MICS; m++) {
    long mapped = map((long)maxDelta[m], 0, 50, 512, 1023);
    if (mapped < 512)   mapped = 512;
    if (mapped > 1023)  mapped = 1023;
    loudness[m] = (int)mapped;
  }

  // Para Serial Plotter: dos líneas en la misma gráfica
  // (Arduino usa el índice como eje X)
  Serial.print(loudness[0]);
  Serial.print(",");
  Serial.println(loudness[1]);

  delay(5);
}
