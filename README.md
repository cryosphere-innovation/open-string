# OpenString

<!-- SPDX-License-Identifier: CC-BY-SA-4.0 -->
<!-- Copyright (c) 2026 Cryosphere Innovation (a wholly owned subsidiary of JASCO Applied Sciences). -->
<!-- Contact: Dr. Cameron Planck <cjp@cryosphereinnovation.com> -->

OpenString is an open-source temperature string for environmental monitoring.

Everything you need to build, flash, and deploy one is in this repo.

## Repository layout

| Path        | Contents                                                     |
| ----------- | ------------------------------------------------------------ |
| `hardware/` | KiCad project, schematic, PCB, Gerbers, BOM, fab-house notes |
| `firmware/` | Arduino sketch for the microcontroller (single `.ino` file)  |
| `docs/`     | Markdown (MDX) source for the OpenString website             |
| `LICENSES/` | Full text of each license used in the project                |
| `LICENSE`   | Top-level license map                                        |

## Build your own

1. Read [`docs/ordering.mdx`](./docs/ordering.mdx) to order the PCB and parts.
2. Follow [`docs/assembly.mdx`](./docs/assembly.mdx) to solder, pot, and seal the string.
3. Flash the firmware (see below, or [`docs/firmware.mdx`](./docs/firmware.mdx)).
4. Deploy.

## Hardware

The KiCad project and fabrication outputs live in `hardware/`:

- `hardware/board-files/` — KiCad project (schematic, PCB layout, project settings)
- `hardware/fabrication/gerbers/` — Gerbers and drill files ready to upload to a PCB house

See [`docs/ordering.mdx`](./docs/ordering.mdx) for the current ordering guide
(fab house settings, stencil, assembly options, expected cost).

Hardware design files are released under **CERN-OHL-S 2.0**
([`LICENSES/CERN-OHL-S-2.0.txt`](./LICENSES/CERN-OHL-S-2.0.txt)).

## Firmware

Single-file Arduino sketch at
[`firmware/src/open_string.ino`](./firmware/src/open_string.ino). PlatformIO
project config at [`platformio.ini`](./platformio.ini). Target board:
**Adafruit Feather M0** (SAMD21).

### Building with PlatformIO (recommended)

1. Install [PlatformIO Core](https://docs.platformio.org/page/core/installation.html)
   (or the PlatformIO IDE extension for VS Code).
2. From the repo root (the folder containing `platformio.ini`):
   ```sh
   pio run                  # compile
   pio run --target upload  # flash over USB
   pio device monitor       # open serial console at 115200 baud
   ```
   PlatformIO installs the SAMD board support and the OneWire library
   automatically based on `platformio.ini`.

   In VS Code, open the repo root as the workspace folder so the
   PlatformIO extension can find `platformio.ini`.

### Building with the Arduino IDE

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) (or
   `arduino-cli`) and the SAMD board support package.
2. The Arduino IDE expects the sketch folder name to match the `.ino`
   filename — rename `firmware/src/` to `firmware/open_string/` (or
   symlink it) and open `open_string.ino` from there.
3. Select **Adafruit Feather M0** as the board, pick the right port,
   and click **Upload**.

See [`docs/firmware.mdx`](./docs/firmware.mdx) for the full flashing
guide, including fuse settings and troubleshooting.

Firmware source is released under **GPL-3.0**
([`LICENSES/GPL-3.0.txt`](./LICENSES/GPL-3.0.txt)).

## License

OpenString is multi-licensed. See [`LICENSE`](./LICENSE) for the full map.
Summary:

- **Hardware** — CERN-OHL-S 2.0
- **Firmware** — GPL-3.0
- **Documentation** — CC-BY-SA-4.0

All three allow commercial use and require attribution. Derivative works
must remain open under the same license.

## Contributing

Issues and pull requests welcome. By contributing, you agree to license
your contribution under the license that applies to the directory you
are contributing to (see [`LICENSE`](./LICENSE)).

## Maintainer and contact

OpenString is maintained by **Cryosphere Innovation** (a wholly owned
subsidiary of JASCO Applied Sciences). Primary contact:
**Dr. Cameron Planck** — <cjp@cryosphereinnovation.com>.

Contributors are listed in [`AUTHORS.md`](./AUTHORS.md).
