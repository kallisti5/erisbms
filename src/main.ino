/*
 * ErisBMS - Flexible battery management system
 *
 * Copyright, 2017 Alexander von Gluck IV
 * Released under the terms of the MIT license.
 *
 */

// Status LED (optional)
#define STATUS_PIN 13

// Battery Pack Voltage (main relay / input monitoring)
#define BPV_RELAY_PIN 12
#define BPV_MONITOR_PIN A5

// Cell Conditions
//  Minimum acceptable cell voltage before BPV cut-off
#define V_MIN 3.0
//  Maximum acceptable cell voltage before dump resistor
#define V_MAX 4.15
//  Maximum acceptable cell voltage before BPV cut-off
#define V_CRITICAL 4.21

// Pin Assignments
//   Number of series cell groups  (1S, 2S, etc)
#define CELL_COUNT 3
//
// Cell pin assignments, number of entries should match CELL_COUNT
//
//   Digital pins with dump relays
int DumpPins[] = {
 2, 3, 4
};
//
//   Analog pins monitoring cell voltages
int MonitorPins[] = {
 A0, A1, A2
};

uint8_t DumpState[CELL_COUNT] = {LOW};
uint8_t BPVRelay = LOW;

void setup()
{
  #ifdef STATUS_PIN
  pinMode(STATUS_PIN, OUTPUT);
  digitalWrite(STATUS_PIN, LOW);
  #endif

  // Setup and disable main BPV relay until we
  // check our batteries.
  pinMode(BPV_RELAY_PIN, OUTPUT);
  digitalWrite(BPV_RELAY_PIN, LOW);

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

void status_flick(int time)
{
  #ifdef STATUS_PIN
  digitalWrite(STATUS_PIN, HIGH);
  delay(time);
  digitalWrite(STATUS_PIN, LOW);
  #endif
}

void update_dumprelay_pins()
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

void update_bpvrelay_pin()
{
  // BPV Relay is N.O.

  if (BPVRelay == HIGH)
    Serial.print("I:Battery Pack Online\n");
  else
    Serial.print("I:Battery Pack Offline\n");

  digitalWrite(BPV_RELAY_PIN, BPVRelay);
}

void loop()
{
  float minimum = V_MAX;
  float maximum = V_MAX;

  for (int i = 0; i < CELL_COUNT; i++) {
    status_flick(5);
    float v = analogRead(MonitorPins[i]) * (5.0 / 1023.0);

    minimum = min(minimum, v);
    maximum = max(maximum, v);

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

  if (maximum > V_CRITICAL || minimum < V_MIN)
    BPVRelay = LOW;
  else
    BPVRelay = HIGH;

  update_bpvrelay_pin();
  update_dumprelay_pins();

  delay(500);
}
