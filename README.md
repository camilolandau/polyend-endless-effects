# Polyend Endless Effects

A collection of custom effects for the [Polyend Endless](https://polyend.com/endless/) guitar pedal.

## Effects

| Effect | Description |
|--------|-------------|
| [Tremvolope](effects/tremvolope/) | Envelope-controlled stereo tremolo. Dynamics shape the rate, depth, and stereo spread — quiet signals bloom into swirling modulation, or loud attacks drive aggressive throb. |

## How to Use

### Quick start (prebuilt binaries)

1. Download a `.endl` file from [Releases](../../releases)
2. Connect your Endless via USB
3. Copy the `.endl` file to the Endless drive

### Build from source

1. Clone the [Polyend FxPatch SDK](https://github.com/polyend/FxPatchSDK)
2. Copy the effect's `PatchImpl.cpp` into the SDK's `source/` directory, replacing the existing file
3. Build:
   ```
   make
   ```
4. Copy the resulting `.endl` file from `build/` to your Endless via USB

Requires the [GNU Arm Embedded Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain).

## Contributing

Have an effect to share? PRs welcome. Add a directory under `effects/` with:
- `PatchImpl.cpp` — the effect source
- `README.md` — description, controls, and sweet spots

## License

MIT
