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
class DelayEffectAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DelayEffectAudioProcessorEditor (DelayEffectAudioProcessor&);
    ~DelayEffectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    void setWaveFormViewerColor(juce::String argb) {};
    const juce::String LOCAL_DEV_SERVER_ADDRESS{ "http://localhost:5173" };
    DelayEffectAudioProcessor& audioProcessor;
    juce::WebSliderRelay dryWetSliderRelay;
    juce::WebSliderRelay delayTimeSliderRelay;
    juce::WebSliderRelay feedbackSliderRelay;
    juce::WebSliderRelay delayTimeFractionSliderRelay;
    juce::WebComboBoxRelay delayTimeTypeComboBoxRelay;
    juce::WebToggleButtonRelay isPingPongToggleButtonRelay;
    juce::WebControlParameterIndexReceiver controlParameterIndexReceiver;
    juce::WebBrowserComponent webBrowserComponent;
    juce::WebSliderParameterAttachment dryWetAttachment;
    juce::WebSliderParameterAttachment delayTimeAttachment;
    juce::WebSliderParameterAttachment feedbackAttachment;
    juce::WebSliderParameterAttachment delayTimeFractionAttachment;
    juce::WebComboBoxParameterAttachment delayTimeTypeAttachment;
    juce::WebToggleButtonParameterAttachment isPingPongAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayEffectAudioProcessorEditor)
};
