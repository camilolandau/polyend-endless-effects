# Necessary Delay

**A bucket-brigade delay into spring reverb for the Polyend Endless.**

Your dry signal stays clean. The repeats don't. Necessary Delay runs a BBD-style analog delay — complete with vibrato wobble and progressive high-frequency rolloff — into a spring reverb that only the delay trails ever touch. The result is a delay that starts articulate and dissolves into dripping, metallic ambience as the repeats fade.

The vibrato modulation gives each repeat a slight pitch drift, the way a real bucket-brigade chip would. The one-pole filter in the feedback path darkens every pass, so early repeats are clear and present while later ones smear into warm fog. Then the spring catches all of that and adds its own resonant, boingy tail — diffused through allpass filters for that characteristic drip, sustained by damped comb filters for the decay.

Hold the footswitch and the repeats freeze into infinite sustain, layering new playing on top of a living, breathing loop.

## Controls

| Knob | Parameter | Description |
|------|-----------|-------------|
| Left | **Time** | Delay time, 30 ms to 3 seconds. Short for slapback, long for sprawling ambient repeats. |
| Mid | **Mix** | Dry/wet blend. Full CCW is dry, full CW is wet. The dry signal is always clean — the spring reverb only colors the delay trails. |
| Right | **Repeats** | Feedback amount. Low for a single echo, high for cascading, darkening trails that wash into the spring. |

**Footswitch press**: Bypass on/off (LED light blue = active, dim blue = bypassed).

**Footswitch hold**: Toggle infinite repeats. The delay loop sustains indefinitely — play over it, layer textures, build drones. LED turns red so you know it's locked.

## How It Works

The signal chain is intentionally asymmetric:

```
Dry signal ──────────────────────────────────────→ output
                                                     ↑
Input → BBD delay → vibrato mod → spring reverb → wet mix
            ↑              |
            └── feedback ──┘
                (LP filtered, soft limited)
```

- **BBD delay**: Mono delay line with linear-interpolated readout. A 2.5 Hz sine LFO wobbles the read position by ~0.8 ms for that analog pitch drift.
- **Feedback path**: Each repeat passes through a one-pole low-pass (~3 kHz) that progressively darkens the sound. A `tanh` soft limiter prevents runaway, especially during infinite hold.
- **Spring reverb**: 4 cascaded Schroeder allpass filters diffuse the signal (modeling spring dispersion), then 4 parallel comb filters with damped feedback create the resonant tail. Only the delay output feeds the spring — your dry tone stays untouched.

## Sweet Spots

- **Analog slapback**: Time at 8 o'clock (~100 ms), Repeats at 9 o'clock, Mix at 10 o'clock. Quick, warm slap with a hint of spring splash.
- **Tape-echo wash**: Time at noon (~1.5 s), Repeats at 1 o'clock, Mix at noon. Repeats darken and dissolve into reverb. Classic ambient delay.
- **Drip machine**: Time at 10 o'clock (~500 ms), Repeats at 2 o'clock, Mix at 1 o'clock. Spring-drenched repeats that cascade and drip.
- **Infinite drone**: Any time setting, hold footswitch. Layer notes and chords over a frozen, evolving delay loop. The spring reverb smears everything into a lush pad.
- **Lo-fi ghost**: Time at 3 o'clock (~2.5 s), Repeats high, Mix at 9 o'clock. Long, dark, barely-there repeats that haunt the background.

## Build

Requires the [GNU Arm Embedded Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain).

```
make
```

Copy the resulting `.endl` file from `build/` to your Endless via USB.

## License

MIT — see [LICENSE](../../LICENSE).
