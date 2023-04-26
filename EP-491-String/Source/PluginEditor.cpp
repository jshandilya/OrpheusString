/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EP491StringAudioProcessorEditor::EP491StringAudioProcessorEditor (EP491StringAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setFreqSliderWithLabel (lowpassSlider, lowpassLabel, audioProcessor.apvts, "LOWPASS", lowpassAttachment);
    
    setSliderWithLabel (rhoSlider, rhoLabel, audioProcessor.apvts, "RHO", rhoAttachment);
    setSliderWithLabel (decaySlider, decayLabel, audioProcessor.apvts, "DECAY", decayAttachment);
    setSliderWithLabel (tailSlider, tailLabel, audioProcessor.apvts, "TAILOFF", tailAttachment);
    setSliderWithLabel (attackSlider, attackLabel, audioProcessor.apvts, "ATTACK", attackAttachment);
    setSliderWithLabel (pickSlider, pickLabel, audioProcessor.apvts, "PICK", pickAttachment);
    
    setSliderWithLabel (reverbSizeSlider, reverbSizeLabel, audioProcessor.apvts, "REVERBSIZE", reverbSizeAttachment);
    setSliderWithLabel (reverbWidthSlider, reverbWidthLabel, audioProcessor.apvts, "REVERBWIDTH", reverbWidthAttachment);
    setSliderWithLabel (reverbDampSlider, reverbDampLabel, audioProcessor.apvts, "REVERBDAMPING", reverbDampAttachment);
    setSliderWithLabel (reverbDrySlider, reverbDryLabel, audioProcessor.apvts, "REVERBDRY", reverbDryAttachment);
    setSliderWithLabel (reverbWetSlider, reverbWetLabel, audioProcessor.apvts, "REVERBWET", reverbWetAttachment);
    setSliderWithLabel (reverbFreezeSlider, reverbFreezeLabel, audioProcessor.apvts, "REVERBFREEZE", reverbFreezeAttachment);
    
    setMixSliderWithLabel (outputSlider, outputLabel, audioProcessor.apvts, "OUTPUT", outputAttachment);
    
    setSize (650, 400);
}

EP491StringAudioProcessorEditor::~EP491StringAudioProcessorEditor()
{
}

//==============================================================================
void EP491StringAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void EP491StringAudioProcessorEditor::resized()
{
    const auto sliderWidth = 100;
    const auto sliderHeight = 100;
    
    const auto labelYOffset = 20;
    const auto labelHeight = 20;
    
    attackSlider.setBounds (25, 50, sliderWidth, sliderHeight);
    attackLabel.setBounds (attackSlider.getX(), attackSlider.getY() - labelYOffset, attackSlider.getWidth(), labelHeight);
    
    decaySlider.setBounds (125, 50, sliderWidth, sliderHeight);
    decayLabel.setBounds (decaySlider.getX(), decaySlider.getY() - labelYOffset, decaySlider.getWidth(), labelHeight);
    
    rhoSlider.setBounds (225, 50, sliderWidth, sliderHeight);
    rhoLabel.setBounds (rhoSlider.getX(), rhoSlider.getY() - labelYOffset, rhoSlider.getWidth(), labelHeight);
    
    tailSlider.setBounds (325, 50, sliderWidth, sliderHeight);
    tailLabel.setBounds (tailSlider.getX(), tailSlider.getY() - labelYOffset, tailSlider.getWidth(), labelHeight);
    
    pickSlider.setBounds (425, 50, sliderWidth, sliderHeight);
    pickLabel.setBounds (pickSlider.getX(), pickSlider.getY() - labelYOffset, pickSlider.getWidth(), labelHeight);
    
    lowpassSlider.setBounds (525, 50, sliderWidth, sliderHeight);
    lowpassLabel.setBounds (lowpassSlider.getX(), lowpassSlider.getY() - labelYOffset, lowpassSlider.getWidth(), labelHeight);
    
    reverbSizeSlider.setBounds (25, 200, sliderWidth, sliderHeight);
    reverbSizeLabel.setBounds (reverbSizeSlider.getX(), reverbSizeSlider.getY() - labelYOffset, reverbSizeSlider.getWidth(), labelHeight);
    
    reverbWidthSlider.setBounds (125, 200, sliderWidth, sliderHeight);
    reverbWidthLabel.setBounds (reverbWidthSlider.getX(), reverbWidthSlider.getY() - labelYOffset, reverbWidthSlider.getWidth(), labelHeight);
    
    reverbDampSlider.setBounds (225, 200, sliderWidth, sliderHeight);
    reverbDampLabel.setBounds (reverbDampSlider.getX(), reverbDampSlider.getY() - labelYOffset, reverbDampSlider.getWidth(), labelHeight);
    
    reverbDrySlider.setBounds (325, 200, sliderWidth, sliderHeight);
    reverbDryLabel.setBounds (reverbDrySlider.getX(), reverbDrySlider.getY() - labelYOffset, reverbDrySlider.getWidth(), labelHeight);
    
    reverbWetSlider.setBounds (425, 200, sliderWidth, sliderHeight);
    reverbWetLabel.setBounds (reverbWetSlider.getX(), reverbWetSlider.getY() - labelYOffset, reverbWetSlider.getWidth(), labelHeight);
    
    reverbFreezeSlider.setBounds (525, 200, sliderWidth, sliderHeight);
    reverbFreezeLabel.setBounds (reverbFreezeSlider.getX(), reverbFreezeSlider.getY() - labelYOffset, reverbFreezeSlider.getWidth(), labelHeight);
    
    outputSlider.setBounds (20, 300, 600, 100);
//    outputLabel.setBounds (outputSlider.getleft, outputSlider.getY(), outputSlider.getWidth(), labelHeight);
}

void EP491StringAudioProcessorEditor::setSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<SliderAttachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 25);
    addAndMakeVisible(slider);
    
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
    
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}

void EP491StringAudioProcessorEditor::setFreqSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<SliderAttachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    addAndMakeVisible(slider);
    
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
    
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    
    slider.setTextValueSuffix (" Hz");
    
    addAndMakeVisible(label);
}

void EP491StringAudioProcessorEditor::setMixSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<SliderAttachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 75, 25);
    addAndMakeVisible(slider);
    
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
    
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    
    slider.setTextValueSuffix (" dB");
    
    addAndMakeVisible(label);
}
