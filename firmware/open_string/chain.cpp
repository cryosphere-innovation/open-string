// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 The OpenString Authors.
//
// DS28EA00 chain-mode discovery.
//
// The Chain command (0x99) takes a two-byte subcommand whose second
// byte is the bitwise complement of the first. Chain state transitions:
//
//     Chain OFF  -> Chain ON   -> Chain DONE
//
// In Chain ON state, exactly one chip on the bus has its EN input
// asserted at any given moment (the one whose PIOB is low). That chip
// alone responds to CHAIN_READ_ROM. When it finishes, it drives its
// own PIOA, which becomes the next chip's PIOB input, handing the
// token down the cable. Walking the chain therefore yields ROMs in
// physical order.
//
// Verify the subcommand bytes and CHAIN_READ_ROM against the
// DS28EA00 datasheet's FUNCTION COMMANDS table before first silicon
// test. The values here match widely-cited Maxim references.

#include "chain.h"

#define CHAIN_CMD           0x99

#define CHAIN_OFF_A         0x3C
#define CHAIN_OFF_B         0xC3
#define CHAIN_ON_A          0x5A
#define CHAIN_ON_B          0xA5
#define CHAIN_DONE_A        0x96
#define CHAIN_DONE_B        0x69

// Command read by the currently-enabled chain chip. Datasheet-verify;
// alternate candidate is 0x0F on some Maxim revisions.
#define CHAIN_READ_ROM      0xEC

#define CMD_SKIP_ROM        0xCC

static void send_chain(OneWire& bus, uint8_t a, uint8_t b) {
  bus.write(CHAIN_CMD);
  bus.write(a);
  bus.write(b);
}

bool chain_discover(OneWire& bus, OpenStringChain& out) {
  out.count = 0;

  // (1) Reset any previous chain state on every chip.
  if (!bus.reset()) return false;
  bus.write(CMD_SKIP_ROM);
  send_chain(bus, CHAIN_OFF_A, CHAIN_OFF_B);

  // (2) Enter Chain ON. Only the chip at the head of the cable
  //     (PIOB tied to GND) is enabled at this point.
  if (!bus.reset()) return false;
  bus.write(CMD_SKIP_ROM);
  send_chain(bus, CHAIN_ON_A, CHAIN_ON_B);

  // (3) Read ROMs one chip at a time, in physical order.
  for (uint8_t i = 0; i < MAX_SENSORS; i++) {
    if (!bus.reset()) break;
    send_chain(bus, CHAIN_ON_A, CHAIN_ON_B);
    bus.write(CHAIN_READ_ROM);

    uint8_t rom[8];
    for (uint8_t b = 0; b < 8; b++) rom[b] = bus.read();

    if (rom[0] == 0x00 || rom[0] == 0xFF) break;
    if (OneWire::crc8(rom, 7) != rom[7]) break;

    memcpy(out.roms[out.count], rom, 8);
    out.count++;
  }

  // (4) Return all chips to normal 1-Wire operation.
  if (bus.reset()) {
    bus.write(CMD_SKIP_ROM);
    send_chain(bus, CHAIN_DONE_A, CHAIN_DONE_B);
  }

  return out.count > 0;
}
