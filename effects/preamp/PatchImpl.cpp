#include "Patch.h"

#include <cmath>

// Transparent preamp with soft saturation and tilt EQ.
//
// Left knob:  Gain (0-20 dB input drive with tanh soft saturation)
// Mid knob:   Tone (tilt EQ: CCW = warm, center = flat, CW = bright)
// Right knob: Volume (output level, 0 to +6 dB)

class PatchImpl : public Patch
{
  public:
    void init() override
    {
        lpStateL = 0.0f;
        lpStateR = 0.0f;
    }

    void setWorkingBuffer(std::span<float, kWorkingBufferSize> /* buffer */) override {}

    void processAudio(std::span<float> audioBufferLeft,
                      std::span<float> audioBufferRight) override
    {
        if (!active)
            return;

        // Gain: 0 dB to 20 dB (1x to 10x)
        const float gain = 1.0f + gainParam * 9.0f;

        // Tone: tilt amount (-1 = dark, 0 = flat, +1 = bright)
        const float tilt = (toneParam - 0.5f) * 2.0f;

        // Volume: 0 to +6 dB (0x to 2x)
        const float volume = volumeParam * 2.0f;

        // One-pole LPF coefficient for tilt EQ pivot (~800 Hz at 48 kHz)
        constexpr float kTiltAlpha = 0.1f;

        for (auto leftIt = audioBufferLeft.begin(), rightIt = audioBufferRight.begin();
             leftIt != audioBufferLeft.end();
             ++leftIt, ++rightIt)
        {
            // --- Gain stage with soft saturation ---
            float left = std::tanh(*leftIt * gain);
            float right = std::tanh(*rightIt * gain);

            // --- Tilt EQ ---
            lpStateL += kTiltAlpha * (left - lpStateL);
            lpStateR += kTiltAlpha * (right - lpStateR);

            left += tilt * (left - lpStateL);
            right += tilt * (right - lpStateR);

            // --- Output volume ---
            *leftIt = left * volume;
            *rightIt = right * volume;
        }
    }

    ParameterMetadata getParameterMetadata(int paramIdx) override
    {
        switch (paramIdx)
        {
            case 0: return { 0.0f, 1.0f, 0.3f };  // Gain (default: mild warmth)
            case 1: return { 0.0f, 1.0f, 0.5f };  // Tone (default: flat)
            case 2: return { 0.0f, 1.0f, 0.5f };  // Volume (default: unity)
            default: return { 0.0f, 1.0f, 0.5f };
        }
    }

    Color getStateLedColor() override { return active ? Color::kPastelGreen : Color::kDimGreen; }

    void setParamValue(int idx, float value) override
    {
        switch (idx)
        {
            case 0: gainParam = value; break;
            case 1: toneParam = value; break;
            case 2: volumeParam = value; break;
        }
    }

    void handleAction(int /* idx */) override { active = !active; }

  private:
    float lpStateL = 0.0f;
    float lpStateR = 0.0f;
    float gainParam = 0.3f;
    float toneParam = 0.5f;
    float volumeParam = 0.5f;
    bool active = true;
};

static PatchImpl patch;

Patch* Patch::getInstance()
{
    return &patch;
}
