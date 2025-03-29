/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class DelayEffectAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    DelayEffectAudioProcessor();
    ~DelayEffectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState audioProcessorValueTreeState;
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    juce::AudioVisualiserComponent waveFormViewer;

private:
    struct Parameters
    {
        juce::AudioParameterFloat* dryWet{ nullptr };
        juce::AudioParameterFloat* delayTime{ nullptr };
        juce::AudioParameterFloat* delayTimeFraction{ nullptr };
        juce::AudioParameterFloat* feedback{ nullptr };
        juce::AudioParameterChoice* delayTimeType{ nullptr };
    };

    juce::AudioProcessorValueTreeState::ParameterLayout createParams(Parameters&);
    Parameters parameters;

    float dryWet{ 0.5f };
    float feedback{ 0.5f };
    float delayTime{ 0.5f };
    float bpm{ 120.0f };
    float delayTimeTypeIndex{ 0.0f };
    float delayTimeFractionIndex{ 0.0f };
    float delayTimeFractionValues[5] = {1.0f, 2.0f, 4.0f, 8.0f, 16.0f};
    juce::dsp::ProcessSpec processSpec;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayEffectAudioProcessor)
};
