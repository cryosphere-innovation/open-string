// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 The OpenString Authors.

#ifndef OPEN_STRING_CONFIG_H
#define OPEN_STRING_CONFIG_H

// 1-Wire data pin (DQ). Requires an external 4.7 kOhm pull-up to 3V3.
#define DATA_PIN            6

// Maximum DS28EA00 sensors supported on one string.
// Storage cost: MAX_SENSORS * 8 bytes for the ordered ROM array.
#define MAX_SENSORS         32

// DS28EA00 worst-case conversion time at 12-bit resolution.
#define CONVERSION_MS       750

// Interval between reading cycles. Must be >= CONVERSION_MS.
#define SAMPLE_INTERVAL_MS  10000

// Serial baud for the human-readable console.
#define SERIAL_BAUD         115200

#endif
