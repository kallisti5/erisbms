/*
 * ErisBMS - Flexible battery management system
 *
 * Copyright, 2017 Alexander von Gluck IV
 * Released under the terms of the MIT license.
 *
 */

#define STATUS_PIN 13

////////////////////////
/////// Pin Assignments
////////////////////////
//
// DumpPins and MonitorPins should always have
// the same number of elemants!
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

void setup() {
  #ifdef STATUS_PIN
  pinMode(STATUS_PIN, OUTPUT);
  digitalWrite(STATUS_PIN, LOW);
  #endif

  // Set relay dump pins to output
  for (int i = 0; i < CELL_COUNT; i++) {
    pinMode(DumpPins[i], OUTPUT);
  }
  // Set monitor pins to input
  for (int i = 0; i < CELL_COUNT; i++) {
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

void loop() {
  for (int i = 0; i < CELL_COUNT; i++) {
    status_flick(10);
    int v = analogRead(MonitorPins[i]);
    delay(100);
  }
}
