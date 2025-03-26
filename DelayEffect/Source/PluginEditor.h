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
    DelayEffectAudioProcessor& audioProcessor;
    juce::WebSliderRelay dryWetSliderRelay;
    juce::WebSliderRelay delayTimeSliderRelay;
    juce::WebSliderRelay feedbackSliderRelay;
    juce::WebComboBoxRelay delayTimeTypeComboBoxRelay;
    juce::WebControlParameterIndexReceiver controlParameterIndexReceiver;
    juce::WebBrowserComponent webBrowserComponent;
    juce::WebSliderParameterAttachment dryWetAttachment;
    juce::WebSliderParameterAttachment delayTimeAttachment;
    juce::WebSliderParameterAttachment feedbackAttachment;
    juce::WebComboBoxParameterAttachment delayTimeTypeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayEffectAudioProcessorEditor)
};
