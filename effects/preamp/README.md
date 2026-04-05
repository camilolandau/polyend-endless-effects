# Preamp

**A transparent preamp and clean boost for the Polyend Endless.**

Sometimes you don't want an effect — you want your guitar to sound more like itself. Preamp is a simple gain stage with soft saturation and a tilt EQ that adds body, presence, or both without ever stepping on your tone. Use it as an always-on front-of-chain foundation, a clean boost to push your amp, or a tone shaper that warms up single coils or tames humbuckers.

At low gain it's virtually invisible. Push it and you get gentle, tube-like compression — the kind of warmth that makes you play better without knowing why. The tilt EQ pivots around 800 Hz, giving you a single knob that goes from round and warm to open and airy without ever sounding harsh.

## Controls

| Knob | Parameter | Description |
|------|-----------|-------------|
| Left | **Gain** | Input drive (0–20 dB). Low settings are transparent clean boost. Higher settings introduce soft saturation — gentle compression and subtle harmonic content, never harsh. |
| Mid | **Tone** | Tilt EQ centered at 800 Hz. At noon it's flat. Turn left for warmth (cuts highs, rounds out the low end). Turn right for presence and sparkle. |
| Right | **Volume** | Output level (silence to +6 dB). Set your final level here after dialing in gain and tone. |

**Footswitch**: Bypass on/off (LED green = active, dim green = bypassed).

## Sweet Spots

- **Always-on foundation**: Gain at 9 o'clock, Tone at noon, Volume to taste. Transparent signal conditioning that just makes everything feel better.
- **Clean boost**: Gain low, Volume at 2 o'clock. Push your amp or downstream pedals without coloring the signal.
- **Warm up single coils**: Gain at noon, Tone at 9 o'clock, Volume at noon. Rounds out the ice-pick highs and adds body without losing clarity.
- **Studio polish**: Gain at 10 o'clock, Tone at 1 o'clock. Opens up the top end with just a touch of saturation glue.
- **Edge of breakup**: Gain at 3 o'clock, Tone to taste, Volume rolled back. As close to overdrive as this gets — responsive, dynamic, cleans up when you back off the guitar volume.

## Build

Requires the [GNU Arm Embedded Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain).

```
make
```

Copy the resulting `.endl` file from `build/` to your Endless via USB.

## License

MIT — see [LICENSE](../../LICENSE).
