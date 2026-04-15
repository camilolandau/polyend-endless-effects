# Polyend Endless Effects

A growing collection of custom guitar effects for the [Polyend Endless](https://polyend.com/endless/) pedal. Every effect includes prebuilt `.endl` binaries — just download and copy to your pedal via USB. Source code included for tweaking and learning.

Built with the [Polyend FxPatch SDK](https://github.com/polyend/FxPatchSDK) for the ARM Cortex-M7 platform.

## Effects

| Effect | Type | Description |
|--------|------|-------------|
| [Tremvolope](effects/tremvolope/) | Tremolo | Envelope-controlled stereo tremolo. Your playing dynamics shape the rate, depth, and stereo spread — quiet signals bloom into swirling modulation, or loud attacks drive aggressive throb. Inspired by the EHX Super Pulsar. |
| [Preamp](effects/preamp/) | Boost / Preamp | Transparent preamp and clean boost with tanh soft saturation and tilt EQ. Adds warmth and compression without coloring your tone. Use as an always-on foundation or to push your amp. |
| [Necessary Delay](effects/necessary-delay/) | Delay / Reverb | BBD-style bucket-brigade delay into spring reverb. Vibrato-wobbled, progressively darkening repeats dissolve into dripping, metallic ambience. Hold the footswitch for infinite sustain, drones, and layering. |

## Download

Prebuilt `.endl` files are available on the [Releases](https://github.com/camilolandau/polyend-endless-effects/releases) page.

1. Download the `.endl` file for the effect you want
2. Connect your Endless to your computer via USB
3. Copy the `.endl` file to the Endless drive
4. Play

No toolchain or compilation needed.

## Build from Source

If you want to tweak an effect or learn how it works:

1. Clone the [Polyend FxPatch SDK](https://github.com/polyend/FxPatchSDK)
2. Copy the effect's `PatchImpl.cpp` into the SDK's `source/` directory, replacing the existing file
3. Install the [GNU Arm Embedded Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)
4. Build:
   ```
   make
   ```
5. Copy the resulting `.endl` file from `build/` to your Endless via USB

## Contributing

Have an effect to share? PRs welcome. Add a directory under `effects/` with:
- `PatchImpl.cpp` — the effect source
- `README.md` — description, controls, and sweet spots

Questions, ideas, or feedback? Open a thread in [Discussions](https://github.com/camilolandau/polyend-endless-effects/discussions).

## License

MIT
