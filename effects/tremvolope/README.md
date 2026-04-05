# Tremvolope

**An envelope-controlled stereo tremolo for the Polyend Endless.**

Tremvolope starts where traditional tremolo ends. At its core it's a lush, sine-wave tremolo with full stereo field — but the third knob opens up a world of expressive, dynamically responsive modulation that reacts to how you play.

Hit a chord hard and the tremolo stays slow, subtle, centered. Let it ring out and decay, and the effect comes alive — the rate climbs, the depth swells, and the stereo image widens into a swirling, immersive pulse that blooms as your notes fade. It's the kind of behavior you'd chase with an expression pedal, except it happens automatically, following the natural envelope of your playing.

Think EHX Super Pulsar territory, but in a single compact patch.

## Controls

| Knob | Parameter | Description |
|------|-----------|-------------|
| Left | **Rate** | Base tremolo speed (1--15 Hz). This is your starting point — the rate you hear when the envelope isn't active. |
| Mid | **Depth** | Base tremolo depth. Dial in anything from a gentle shimmer to a hard chop. |
| Right | **Envelope** | The magic knob. At noon it's a straight tremolo. Turn it left and quieter signals push the rate faster, the depth deeper, and the stereo wider — perfect for swells that bloom as they decay. Turn it right for the inverse: loud playing drives the intensity up, giving you aggressive attack-responsive throb. |

**Footswitch**: Bypass on/off (LED blue = active, dim blue = bypassed).

## How It Works

A real-time envelope follower tracks your input signal's amplitude with fast attack (~5 ms) and slow release (~300 ms), capturing the natural dynamics of your playing. The envelope knob controls how much — and in which direction — that amplitude shapes three parameters at once:

- **Rate**: Accelerates from your base setting toward 15 Hz
- **Depth**: Deepens toward full wet
- **Stereo spread**: The left and right LFOs separate in phase, from mono (0 degrees) up to full ping-pong (180 degrees)

The center-detent design means you can always dial in a classic tremolo and then season to taste in either direction.

## Sweet Spots

- **Slow bloom**: Rate at 9 o'clock, Depth at noon, Envelope at 8 o'clock. Strum a big open chord and let it decay into a washy, accelerating stereo pulse.
- **Straight classic**: Rate to taste, Depth to taste, Envelope at noon. Pure sine tremolo, no envelope, no frills.
- **Attack throb**: Rate at 10 o'clock, Depth at 2 o'clock, Envelope at 2 o'clock. Dig in hard for aggressive, fast tremolo that mellows out as you ease up.
- **Ambient swell**: Rate at 7 o'clock, Depth at 3 o'clock, Envelope fully CCW. Pair with reverb. Let notes hang and dissolve into rhythmic stereo movement.

## Build

Requires the [GNU Arm Embedded Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain).

```
make
```

Copy the resulting `.endl` file from `build/` to your Endless via USB.

## License

MIT — see [LICENSE.TXT](LICENSE.TXT).
