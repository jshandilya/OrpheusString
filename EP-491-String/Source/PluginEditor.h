/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class EP491StringAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    EP491StringAudioProcessorEditor (EP491StringAudioProcessor&);
    ~EP491StringAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    using BoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    juce::Slider lowpassSlider, rhoSlider, decaySlider;
    std::unique_ptr<SliderAttachment> lowpassAttachment, rhoAttachment, decayAttachment;
    juce::Label lowpassLabel { "Filter", "Filter" }, rhoLabel { "Sustain", "Sustain" }, decayLabel { "Decay", "Decay" };
    
    juce::Slider tailSlider, attackSlider, pickSlider;
    std::unique_ptr<SliderAttachment> tailAttachment, attackAttachment, pickAttachment;
    juce::Label tailLabel { "Release", "Release" }, attackLabel { "Attack", "Attack" }, pickLabel { "Pick", "Pick" };
    
    juce::Slider reverbSizeSlider, reverbWidthSlider, reverbDampSlider;
    std::unique_ptr<SliderAttachment> reverbSizeAttachment, reverbWidthAttachment, reverbDampAttachment;
    juce::Label reverbSizeLabel { "Reverb Size", "Reverb Size" }, reverbWidthLabel { "Reverb Width", "Reverb Width" }, reverbDampLabel { "Reverb Damping", "Reverb Damping" };
    
    juce::Slider reverbDrySlider, reverbWetSlider, reverbFreezeSlider;
    std::unique_ptr<SliderAttachment> reverbDryAttachment, reverbWetAttachment, reverbFreezeAttachment;
    juce::Label reverbDryLabel { "Reverb Dry", "Reverb Dry" }, reverbWetLabel { "Reverb Wet", "Reverb Wet" }, reverbFreezeLabel { "Reverb Freeze", "Reverb Freeze" };
    
    juce::Slider outputSlider;
    std::unique_ptr<SliderAttachment> outputAttachment;
    juce::Label outputLabel { "Output", "Output" };
    
    void setSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<SliderAttachment>& attachment);
    
    void setMixSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<SliderAttachment>& attachment);
    
    
    EP491StringAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EP491StringAudioProcessorEditor)
};
