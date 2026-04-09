#include "Patch.h"

#include <cmath>

// BBD-style delay into spring reverb.
// Dry signal is never reverbed — only the delay trails hit the spring.
//
// Left knob:  Time (30 ms to 3000 ms)
// Mid knob:   Mix (dry/wet)
// Right knob: Repeats (feedback amount)
// Footswitch press: bypass
// Footswitch hold:  toggle infinite repeats (LED turns red)

constexpr float kPi = 3.14159265f;
constexpr float kTwoPi = 2.0f * kPi;

// Delay
constexpr int kMaxDelaySamples = 144000; // 3000 ms at 48 kHz
constexpr int kMinDelaySamples = 1440;   // 30 ms

// BBD vibrato modulation
constexpr float kVibratoRate = 2.5f;  // Hz
constexpr float kVibratoDepth = 40.0f; // samples (~0.8 ms)
constexpr float kBbdDamping = 0.35f;   // LP coefficient per repeat (~3 kHz)

// Spring reverb — allpass diffusers (series)
constexpr int kAllpassLengths[] = {556, 441, 341, 225};
constexpr int kNumAllpasses = 4;
constexpr float kAllpassCoeff = 0.5f;

// Spring reverb — comb filters (parallel)
constexpr int kCombLengths[] = {1687, 1601, 2053, 2251};
constexpr int kNumCombs = 4;
constexpr float kCombDecay = 0.7f;
constexpr float kCombDamping = 0.3f;

class PatchImpl : public Patch
{
  public:
    void init() override
    {
        delayWritePos = 0;
        lfoPhase = 0.0f;
        bbdLpState = 0.0f;

        for (int i = 0; i < kNumCombs; ++i)
        {
            combPos[i] = 0;
            combLpState[i] = 0.0f;
        }
        for (int i = 0; i < kNumAllpasses; ++i)
            allpassPos[i] = 0;
    }

    void setWorkingBuffer(std::span<float, kWorkingBufferSize> buffer) override
    {
        // Partition the working buffer into delay line + reverb buffers
        delayLine = buffer.data();
        int offset = kMaxDelaySamples;

        for (int i = 0; i < kNumCombs; ++i)
        {
            combLines[i] = buffer.data() + offset;
            offset += kCombLengths[i];
        }
        for (int i = 0; i < kNumAllpasses; ++i)
        {
            allpassLines[i] = buffer.data() + offset;
            offset += kAllpassLengths[i];
        }

        // Zero all buffers
        for (int i = 0; i < offset; ++i)
            buffer[i] = 0.0f;
    }

    void processAudio(std::span<float> audioBufferLeft,
                      std::span<float> audioBufferRight) override
    {
        if (!active)
            return;

        const int delaySamples = kMinDelaySamples +
            static_cast<int>(timeParam * static_cast<float>(kMaxDelaySamples - kMinDelaySamples));
        const float feedback = infiniteHold ? 0.99f : repeatsParam * 0.9f;
        const float wet = mixParam;
        const float dry = 1.0f - mixParam;

        for (auto leftIt = audioBufferLeft.begin(), rightIt = audioBufferRight.begin();
             leftIt != audioBufferLeft.end();
             ++leftIt, ++rightIt)
        {
            float input = (*leftIt + *rightIt) * 0.5f;

            // --- Vibrato LFO ---
            lfoPhase += kVibratoRate / static_cast<float>(kSampleRate);
            if (lfoPhase >= 1.0f)
                lfoPhase -= 1.0f;
            float mod = std::sin(kTwoPi * lfoPhase) * kVibratoDepth;

            // --- Read from delay with vibrato + linear interpolation ---
            float readPosF = static_cast<float>(delayWritePos)
                           - static_cast<float>(delaySamples) + mod;
            while (readPosF < 0.0f)
                readPosF += static_cast<float>(kMaxDelaySamples);

            int readIdx = static_cast<int>(readPosF);
            float frac = readPosF - static_cast<float>(readIdx);
            int idx0 = readIdx % kMaxDelaySamples;
            int idx1 = (readIdx + 1) % kMaxDelaySamples;

            float delayed = delayLine[idx0] * (1.0f - frac) + delayLine[idx1] * frac;

            // --- BBD low-pass (darkens with each repeat) ---
            bbdLpState += kBbdDamping * (delayed - bbdLpState);
            delayed = bbdLpState;

            // --- Write to delay (input + feedback with soft limit) ---
            delayLine[delayWritePos] = std::tanh(input + delayed * feedback);
            delayWritePos = (delayWritePos + 1) % kMaxDelaySamples;

            // --- Spring reverb on delay output only ---
            float wetSignal = processSpring(delayed);

            // --- Mix (dry signal never hits the spring) ---
            *leftIt = *leftIt * dry + wetSignal * wet;
            *rightIt = *rightIt * dry + wetSignal * wet;
        }
    }

    ParameterMetadata getParameterMetadata(int paramIdx) override
    {
        switch (paramIdx)
        {
            case 0: return { 0.0f, 1.0f, 0.3f };  // Time
            case 1: return { 0.0f, 1.0f, 0.5f };  // Mix
            case 2: return { 0.0f, 1.0f, 0.4f };  // Repeats
            default: return { 0.0f, 1.0f, 0.5f };
        }
    }

    Color getStateLedColor() override
    {
        if (!active)
            return Color::kDimBlue;
        return infiniteHold ? Color::kRed : Color::kLightBlueColor;
    }

    void setParamValue(int idx, float value) override
    {
        switch (idx)
        {
            case 0: timeParam = value; break;
            case 1: mixParam = value; break;
            case 2: repeatsParam = value; break;
        }
    }

    void handleAction(int actionIdx) override
    {
        if (actionIdx == 0)
            active = !active;
        else if (actionIdx == 1)
            infiniteHold = !infiniteHold;
    }

  private:
    float processSpring(float input)
    {
        // Allpass diffusion chain (spring dispersion)
        float signal = input;
        for (int i = 0; i < kNumAllpasses; ++i)
        {
            float* buf = allpassLines[i];
            int len = kAllpassLengths[i];
            int& pos = allpassPos[i];

            float delayed = buf[pos];
            float temp = signal + kAllpassCoeff * delayed;
            buf[pos] = temp;
            signal = delayed - kAllpassCoeff * temp;
            pos = (pos + 1) % len;
        }

        // Parallel comb filters with damped feedback (resonant tail)
        float combSum = 0.0f;
        for (int i = 0; i < kNumCombs; ++i)
        {
            float* buf = combLines[i];
            int len = kCombLengths[i];
            int& pos = combPos[i];
            float& lp = combLpState[i];

            float delayed = buf[pos];
            lp += kCombDamping * (delayed - lp);
            buf[pos] = signal + kCombDecay * lp;
            combSum += delayed;
            pos = (pos + 1) % len;
        }

        return combSum * 0.25f;
    }

    // Working buffer partitions
    float* delayLine = nullptr;
    float* combLines[kNumCombs] = {};
    float* allpassLines[kNumAllpasses] = {};

    // Delay state
    int delayWritePos = 0;
    float lfoPhase = 0.0f;
    float bbdLpState = 0.0f;

    // Spring reverb state
    int combPos[kNumCombs] = {};
    float combLpState[kNumCombs] = {};
    int allpassPos[kNumAllpasses] = {};

    // Parameters
    float timeParam = 0.3f;
    float mixParam = 0.5f;
    float repeatsParam = 0.4f;

    // State
    bool active = true;
    bool infiniteHold = false;
};

static PatchImpl patch;

Patch* Patch::getInstance()
{
    return &patch;
}
