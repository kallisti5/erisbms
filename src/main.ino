/*
 * ErisBMS - Flexible battery management system
 *
 * Copyright, 2017 Alexander von Gluck IV
 * Released under the terms of the MIT license.
 *
 */

#define STATUS_PIN 13

#define V_MIN 3.0
#define V_MAX 4.2

////////////////////////
/////// Pin Assignments
////////////////////////
//
// DumpPins and MonitorPins should always have
// the same number of elemants!
//
// CELL_COUNT = nS (1S, 2S, 3S, etc)
//
#define CELL_COUNT 3
//
// Digital pins with dump relays
int DumpPins[] = {
 2, 3, 4
};
//
// Analog pins monitoring cell voltages
int MonitorPins[] = {
 A0, A1, A2
};
//
/////////

uint8_t DumpState[CELL_COUNT] = {LOW};

void setup() {
  #ifdef STATUS_PIN
  pinMode(STATUS_PIN, OUTPUT);
  digitalWrite(STATUS_PIN, LOW);
  #endif

  Serial.begin(9600);
  Serial.println("I: ErisBMS - Smartly control chaos");
  Serial.print("I: Version ");
  Serial.print(VERSION);
  Serial.print("\n");

  // Set relay dump pins to output
  for (int i = 0; i < CELL_COUNT; i++) {
    pinMode(DumpPins[i], OUTPUT);
  }
  // Set monitor pins to input
  for (int i = 0; i < CELL_COUNT; i++) {
    Serial.print("I: Setup Cell ");
    Serial.print(i);
    Serial.print("\n");
    pinMode(MonitorPins[i], INPUT);
  }
}

void status_flick(int time) {
  #ifdef STATUS_PIN
  digitalWrite(STATUS_PIN, HIGH);
  delay(time);
  digitalWrite(STATUS_PIN, LOW);
  #endif
}

void update_dumpstates()
{
  for (int i = 0; i < CELL_COUNT; i++) {
    if (DumpState[i] == HIGH) {
      Serial.print("A:Shunt active on cell ");
      Serial.print(i);
      Serial.print("\n");
    }
    digitalWrite(DumpPins[i], DumpState[i]);
  }
}

void loop() {
  float minimum = V_MAX;

  for (int i = 0; i < CELL_COUNT; i++) {
    status_flick(5);
    float v = analogRead(MonitorPins[i]) * (5.0 / 1023.0);
	minimum = min(minimum, v);
    Serial.print("D:");
    Serial.print(i);
    Serial.print(":");
    Serial.print(v);
    Serial.print("\n");
    if (v > V_MAX) {
      DumpState[i] = HIGH;
    } else {
      DumpState[i] = LOW;
    }
  }
  if (minimum < V_MIN) {
    Serial.print("A:Cell depleted @");
    Serial.print(minimum);
    Serial.print("\n");
  }
  delay(500);
  update_dumpstates();
}
