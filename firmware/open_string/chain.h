// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 The OpenString Authors.

#ifndef OPEN_STRING_CHAIN_H
#define OPEN_STRING_CHAIN_H

#include <Arduino.h>
#include <OneWire.h>
#include "config.h"

struct OpenStringChain {
  uint8_t count;
  uint8_t roms[MAX_SENSORS][8];
};

// Walk a DS28EA00 chain end-to-end and fill `out` with the ROMs in
// physical cable order. Returns true if at least one ROM was read.
//
// Preconditions:
//   - All chips on external Vdd (chain mode does NOT work on parasitic).
//   - First chip's PIOB tied to GND; PIOA of chip N wired to PIOB of N+1.
bool chain_discover(OneWire& bus, OpenStringChain& out);

#endif
