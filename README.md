# OpenString

An open-source temperature string for environmental monitoring — lakes,
soil profiles, greenhouses, process tanks, and anywhere else you need
temperature at multiple depths or points along a cable.

Everything you need to build, flash, and deploy one is in this repo.

## Repository layout

| Path         | Contents                                                     |
| ------------ | ------------------------------------------------------------ |
| `hardware/`  | KiCad project, schematic, PCB, Gerbers, BOM, fab-house notes |
| `firmware/`  | Arduino sketch and libraries for the microcontroller         |
| `docs/`      | Markdown (MDX) source rendered on the Cryosphere docs site   |
| `LICENSES/`  | Full text of each license used in the project                |
| `LICENSE`    | Top-level license map                                        |

## Build your own

1. Read `docs/ordering.mdx` to order the PCB and parts.
2. Follow `docs/assembly.mdx` to solder, pot, and seal the string.
3. Follow `docs/firmware.mdx` to flash the Arduino firmware.
4. Deploy.

## License

OpenString is multi-licensed. See [`LICENSE`](./LICENSE) for the full map.
Summary:

- **Hardware** — CERN-OHL-S 2.0
- **Firmware** — GPL-3.0
- **Documentation** — CC-BY-SA-4.0

All three allow commercial use and require attribution. Derivative works
must remain open under the same license.

## How the docs are published

The OpenString documentation lives at
[cryosphereinnovation.com/docs](https://cryosphereinnovation.com/docs).
That site fetches the MDX files in `docs/` directly from this repo at
render time — pushes to `main` show up on the live site within a few
minutes. No build, no submodule bump, no manual sync.

`docs/manifest.json` is generated automatically by a GitHub Action
(`.github/workflows/manifest.yml`) on every push that touches an MDX
file. You don't need to edit it by hand — just add/edit `.mdx` files
and push.

## Contributing

Issues and pull requests welcome. By contributing, you agree to license
your contribution under the license that applies to the directory you
are contributing to (see `LICENSE`).

## Authors

OpenString is built by its contributors. See [`AUTHORS.md`](./AUTHORS.md)
for the list. Copyright is held collectively by **The OpenString Authors**.
