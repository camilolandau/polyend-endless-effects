#include "Patch.h"

#include <cmath>

// Envelope-controlled stereo tremolo.
//
// Left knob:  Rate (1-12 Hz base)
// Mid knob:   Depth
// Right knob: Envelope sensitivity + stereo spread
//             Center = straight mono trem
//             Below center = inverse envelope (quiet -> faster/deeper/wider)
//             Above center = direct envelope (loud -> faster/deeper/wider)

constexpr float kPi = 3.14159265f;
constexpr float kTwoPi = 2.0f * kPi;
constexpr float kMinRate = 1.0f;
constexpr float kMaxRate = 15.0f;

class PatchImpl : public Patch
{
  public:
    void init() override
    {
        phase = 0.0f;
        envelope = 0.0f;
    }

    void setWorkingBuffer(std::span<float, kWorkingBufferSize> /* buffer */) override {}

    void processAudio(std::span<float> audioBufferLeft,
                      std::span<float> audioBufferRight) override
    {
        if (!active)
            return;

        const float baseRate = kMinRate + rateParam * (kMaxRate - kMinRate);
        const float baseDepth = depthParam;

        // Envelope knob: distance from center = amount; direction = mode
        const float envAmount = std::abs(envParam - 0.5f) * 2.0f;
        const bool inverse = envParam < 0.5f;

        for (auto leftIt = audioBufferLeft.begin(), rightIt = audioBufferRight.begin();
             leftIt != audioBufferLeft.end();
             ++leftIt, ++rightIt)
        {
            // --- Envelope follower (peak detector with fast attack / slow release) ---
            float inputLevel = (std::abs(*leftIt) + std::abs(*rightIt)) * 0.5f;
            float coeff = (inputLevel > envelope) ? 0.004f : 0.0001f;
            envelope += coeff * (inputLevel - envelope);

            // Normalize to roughly 0-1 (guitar signals are well under 1.0)
            float envNorm = std::min(envelope * 2.0f, 1.0f);

            // Inverse: quiet signal -> high envMod -> more speed/depth/stereo
            // Direct:  loud signal  -> high envMod -> more speed/depth/stereo
            float envMod = inverse ? (1.0f - envNorm) : envNorm;

            // --- Modulate parameters ---
            float rate = baseRate + envAmount * envMod * (kMaxRate - baseRate);
            float depth = baseDepth + envAmount * envMod * (1.0f - baseDepth);

            // Stereo phase offset: 0 at center, up to 0.5 (180 degrees) at extremes
            float stereoOffset = envAmount * envMod * 0.5f;

            // --- LFO ---
            phase += rate / static_cast<float>(kSampleRate);
            if (phase >= 1.0f)
                phase -= 1.0f;

            float lfoLeft = 0.5f * (1.0f - std::cos(kTwoPi * phase));
            float lfoRight = 0.5f * (1.0f - std::cos(kTwoPi * (phase + stereoOffset)));

            // --- Apply tremolo ---
            *leftIt *= 1.0f - depth * lfoLeft;
            *rightIt *= 1.0f - depth * lfoRight;
        }
    }

    ParameterMetadata getParameterMetadata(int paramIdx) override
    {
        switch (paramIdx)
        {
            case 0: return { 0.0f, 1.0f, 0.3f };  // Rate (default: moderate)
            case 1: return { 0.0f, 1.0f, 0.5f };  // Depth (default: half)
            case 2: return { 0.0f, 1.0f, 0.5f };  // Envelope (default: center/off)
            default: return { 0.0f, 1.0f, 0.5f };
        }
    }

    Color getStateLedColor() override { return active ? Color::kBlue : Color::kDimBlue; }

    void setParamValue(int idx, float value) override
    {
        switch (idx)
        {
            case 0: rateParam = value; break;
            case 1: depthParam = value; break;
            case 2: envParam = value; break;
        }
    }

    void handleAction(int /* idx */) override { active = !active; }

  private:
    float phase = 0.0f;
    float envelope = 0.0f;
    float rateParam = 0.3f;
    float depthParam = 0.5f;
    float envParam = 0.5f;
    bool active = true;
};

static PatchImpl patch;

Patch* Patch::getInstance()
{
    return &patch;
}
