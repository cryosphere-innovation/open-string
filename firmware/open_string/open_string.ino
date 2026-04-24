// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 The OpenString Authors.
//
// OpenString reference firmware for DS28EA00 temperature strings.
// Target: Adafruit Feather M0 (SAMD21), 3.3V logic, USB-native Serial.
//
// On boot this sketch:
//   1. Enumerates all 1-Wire devices by standard ROM search (sanity check).
//   2. Runs DS28EA00 chain mode to discover sensors in physical cable order.
//   3. Loops forever issuing Convert T and reading each sensor's scratchpad.

#include <Arduino.h>
#include <OneWire.h>
#include "config.h"
#include "chain.h"

#define CMD_MATCH_ROM        0x55
#define CMD_SKIP_ROM         0xCC
#define CMD_CONVERT_T        0x44
#define CMD_READ_SCRATCHPAD  0xBE

#define FAMILY_DS28EA00      0x42

static OneWire          bus(DATA_PIN);
static OpenStringChain  chain;

static void print_rom(const uint8_t* rom) {
  for (uint8_t i = 0; i < 8; i++) {
    if (rom[i] < 0x10) Serial.print('0');
    Serial.print(rom[i], HEX);
    if (i != 7) Serial.print(' ');
  }
}

static void print_banner() {
  Serial.println();
  Serial.println("=== OpenString firmware (reference) ===");
  Serial.println("License: GPL-3.0-only");
  Serial.print("Data pin: D"); Serial.println(DATA_PIN);
  Serial.print("Sample interval: "); Serial.print(SAMPLE_INTERVAL_MS); Serial.println(" ms");
  Serial.println();
}

static void search_and_print() {
  Serial.println("Standard 1-Wire bus search:");
  bus.reset_search();
  uint8_t rom[8];
  uint8_t n = 0;
  while (bus.search(rom)) {
    if (OneWire::crc8(rom, 7) != rom[7]) {
      Serial.println("  (bad ROM CRC, skipping)");
      continue;
    }
    n++;
    Serial.print("  ["); print_rom(rom); Serial.print("]");
    if (rom[0] == FAMILY_DS28EA00) Serial.print("  DS28EA00");
    else { Serial.print("  family=0x"); Serial.print(rom[0], HEX); }
    Serial.println();
  }
  Serial.print("Found "); Serial.print(n); Serial.println(" device(s).");
  Serial.println();
}

static void print_chain() {
  Serial.print("Chain-mode discovery: ");
  Serial.print(chain.count);
  Serial.println(" sensor(s) in physical order.");
  for (uint8_t i = 0; i < chain.count; i++) {
    Serial.print("  Sensor "); Serial.print(i + 1); Serial.print(" [");
    print_rom(chain.roms[i]);
    Serial.println("]");
  }
  Serial.println();
}

static float read_temperature(const uint8_t* rom) {
  if (!bus.reset()) return NAN;
  bus.write(CMD_MATCH_ROM);
  for (uint8_t i = 0; i < 8; i++) bus.write(rom[i]);
  bus.write(CMD_READ_SCRATCHPAD);

  uint8_t s[9];
  for (uint8_t i = 0; i < 9; i++) s[i] = bus.read();

  if (OneWire::crc8(s, 8) != s[8]) return NAN;

  int16_t raw = ((int16_t)s[1] << 8) | s[0];
  return raw * 0.0625f;
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  uint32_t t0 = millis();
  while (!Serial && (millis() - t0) < 3000) { ; }

  print_banner();
  search_and_print();

  if (!chain_discover(bus, chain)) {
    Serial.println("Chain discovery failed. Check wiring:");
    Serial.println("  - First chip PIOB tied to GND");
    Serial.println("  - PIOA of chip N wired to PIOB of chip N+1");
    Serial.println("  - External Vdd required (parasitic power not supported)");
    Serial.println();
  } else {
    print_chain();
  }
}

void loop() {
  if (chain.count == 0) {
    Serial.println("No sensors discovered. Idling; power-cycle after fixing wiring.");
    delay(5000);
    return;
  }

  // Broadcast Convert T to every sensor on the bus.
  bus.reset();
  bus.write(CMD_SKIP_ROM);
  bus.write(CMD_CONVERT_T);
  delay(CONVERSION_MS);

  Serial.print("--- OpenString reading at t=");
  Serial.print(millis());
  Serial.println(" ms ---");

  for (uint8_t i = 0; i < chain.count; i++) {
    float c = read_temperature(chain.roms[i]);
    Serial.print("Sensor "); Serial.print(i + 1);
    Serial.print(" ["); print_rom(chain.roms[i]); Serial.print("]: ");
    if (isnan(c)) Serial.println("read error (CRC)");
    else { Serial.print(c, 2); Serial.println(" C"); }
  }
  Serial.println();

  uint32_t remain = (SAMPLE_INTERVAL_MS > CONVERSION_MS)
      ? (SAMPLE_INTERVAL_MS - CONVERSION_MS) : 0;
  delay(remain);
}
