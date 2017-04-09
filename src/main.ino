/*
 * ErisBMS - Flexible battery management system
 *
 * Copyright, 2017 Alexander von Gluck IV
 * Released under the terms of the MIT license.
 *
 */

// Digital pins with dump relays
int DumpPins[] = {
  0
};

void setup() {
  pinMode(DumpPins[0], OUTPUT);
}

void loop() {
  digitalWrite(DumpPins[0], HIGH);
}
