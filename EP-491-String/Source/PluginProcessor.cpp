/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Voice.h"

//==============================================================================
EP491StringAudioProcessor::EP491StringAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParams())
#endif
{
    synth.addSound (new Sound());
    
    for (int i = 0; i < 4; i++)
    {
        synth.addVoice (new Voice());
    }
    
    reverb.reset();
}

EP491StringAudioProcessor::~EP491StringAudioProcessor()
{
    reverb.reset();
}

//==============================================================================
const juce::String EP491StringAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EP491StringAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EP491StringAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EP491StringAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EP491StringAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EP491StringAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EP491StringAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EP491StringAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String EP491StringAudioProcessor::getProgramName (int index)
{
    return {};
}

void EP491StringAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void EP491StringAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    reverb.reset();
    
    synth.setCurrentPlaybackSampleRate (sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    reverbParams.roomSize = 0.5f;
    reverbParams.width = 1.0f;
    reverbParams.damping = 0.5f;
    reverbParams.freezeMode = 0.0f;
    reverbParams.dryLevel = 1.0f;
    reverbParams.wetLevel = 0.0f;
    
    reverb.setParameters (reverbParams);
}

void EP491StringAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EP491StringAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void EP491StringAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<Voice*>(synth.getVoice(i)))
        {
            auto& L = *apvts.getRawParameterValue("LOWPASS");
            auto& rho = *apvts.getRawParameterValue("RHO");
            auto& S = *apvts.getRawParameterValue("DECAY");
            auto& mu = *apvts.getRawParameterValue("PICK");
            auto& t60 = *apvts.getRawParameterValue("TAILOFF");
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            
            voice->setVoiceParams (L, rho, S, mu, t60, attack);
        }
    }

    reverbParams.roomSize = *apvts.getRawParameterValue ("REVERBSIZE");
    reverbParams.width = *apvts.getRawParameterValue ("REVERBWIDTH");
    reverbParams.damping = *apvts.getRawParameterValue ("REVERBDAMPING");
    reverbParams.dryLevel = *apvts.getRawParameterValue ("REVERBDRY");
    reverbParams.wetLevel = *apvts.getRawParameterValue ("REVERBWET");
    reverbParams.freezeMode = *apvts.getRawParameterValue ("REVERBFREEZE");
    
    auto& output = *apvts.getRawParameterValue ("OUTPUT");
    
    reverb.setParameters (reverbParams);
        
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
    juce::dsp::AudioBlock<float> block { buffer };
    reverb.process (juce::dsp::ProcessContextReplacing<float> (block));
    
    auto outputGain = juce::Decibels::decibelsToGain (output.load());
    
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= outputGain;
        }
    }
}

//==============================================================================
bool EP491StringAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* EP491StringAudioProcessor::createEditor()
{
    return new EP491StringAudioProcessorEditor (*this);
}

//==============================================================================
void EP491StringAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EP491StringAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EP491StringAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout EP491StringAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // String params
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "LOWPASS", 1 }, "Lowpass", juce::NormalisableRange<float> { 80.0f, 1400.0f, 0.1f }, 1400.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "RHO", 1 }, "Rho", juce::NormalisableRange<float> { 0.8f, 1.0f, 0.001f }, 1.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "DECAY", 1 }, "Decay", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.001f }, 0.5f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "TAILOFF", 1 }, "Tailoff", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.001f }, 0.5f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "ATTACK", 1 }, "Attack", juce::NormalisableRange<float> { 0.8f, 10.0f, 0.1f }, 0.8f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "PICK", 1 }, "Pick", juce::NormalisableRange<float> { 0.01f, 1.0f, 0.001f }, 0.5f));
    
    // Reverb paarams
    params.push_back (std::make_unique<juce::AudioParameterFloat>("REVERBSIZE", "Reverb Size", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.0f, ""));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("REVERBWIDTH", "Reverb Width", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 1.0f, ""));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("REVERBDAMPING", "Reverb Damping", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.5f, ""));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("REVERBDRY", "Reverb Dry", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 1.0f, ""));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("REVERBWET", "Reverb Wet", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.0f, ""));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("REVERBFREEZE", "Reverb Freeze", juce::NormalisableRange<float> { 0.0f, 1.0f, 1.0f }, 0.0f, ""));
    
    // Mix params
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "OUTPUT", 1 }, "Output", juce::NormalisableRange<float> { -30.f, 10.0f, 0.01f}, 0.0f));
    
    return { params.begin(), params.end() };
}

