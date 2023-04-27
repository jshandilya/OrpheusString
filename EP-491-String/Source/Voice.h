/*
  ==============================================================================

    Voice.h
    Created: 10 Apr 2023 10:16:26pm
    Author:  Jahan Shandilya

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Sound : public juce::SynthesiserSound
{
public:
    Sound() {}
    
    bool appliesToNote (int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};

class Voice : public juce::SynthesiserVoice
{
public:
    Voice() {};
    ~Voice() {};
    
    void startNote(int midiNote, float velocity, juce::SynthesiserSound*, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void pitchWheelMoved(int newPitchWheelValue) override;

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override {};
    
    void setVoiceParams (float L, float rho, float S, float mu, float t60, float pickStrengthFactor);
    
//    float mL = 0.f;
//    float mRho = 0.f;
//    float mS = 0.f;
//    float mMu = 0.f;
//    float mT60 = 0.f;
//    int mPitchBendSemitones = 0;
//    float mPickStrengthFactor = 0.f;
    
    
    float mL = 400.f;
    float mRho = 0.99f;
    float mS = 0.5f;
    float mMu = 0.1f;
    float mT60 = 0.1f;
    int mPitchBendSemitones = 12.f;
    float mPickStrengthFactor = 1.f;
    
private:
    juce::Random rand;

    int currentNote;

    const static int bufferSize = 1 << 16;

    float excitation[bufferSize];
    float lowpassDelayBuffer[bufferSize];
    float allpassTuningBuffer[bufferSize];
    float lowpassDynamicsBuffer[bufferSize];

    int bufIdx = 0;
    int period;

    float upperPitchLimit = 1300;
    float lowerPitchLimit = 80;

    float middlePitchOmega = juce::MathConstants<float>::twoPi * std::sqrt(upperPitchLimit * lowerPitchLimit);

    float R;

    float rhoTailoff;

    int lastTailoffIdx;

    bool playing = false;
    bool tailoff = false;

    float allpassTuningC;

    void calculatePitchInfo(int midiNote, int pwPosition);
    void createExcitation(float velocity);
    
    juce::MidiKeyboardState midiKeyboard;
};
