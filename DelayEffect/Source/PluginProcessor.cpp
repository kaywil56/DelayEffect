/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayEffectAudioProcessor::DelayEffectAudioProcessor() : 
    AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo())
        .withOutput("Output", juce::AudioChannelSet::stereo())),
    audioProcessorValueTreeState(*this, nullptr, "Params", createParams())
{
}

DelayEffectAudioProcessor::~DelayEffectAudioProcessor()
{
}

//==============================================================================
const juce::String DelayEffectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayEffectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayEffectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayEffectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayEffectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayEffectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayEffectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayEffectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayEffectAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayEffectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelayEffectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    delayBufferSamples = 2 * (sampleRate * 2.0);
    delayBuffer.setSize(getTotalNumInputChannels(), delayBufferSamples);
    delayBuffer.clear();

    delayWritePosition = 0;
    this->sampleRate = sampleRate;
}

void DelayEffectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool DelayEffectAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& mainInLayout = layouts.getChannelSet(true, 0);
    const auto& mainOutLayout = layouts.getChannelSet(false, 0);

    return (mainInLayout == mainOutLayout && (!mainInLayout.isDisabled()));
}

void DelayEffectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    const int numChannels = getTotalNumInputChannels();
    const int numSamples = buffer.getNumSamples();

    dryWet = *audioProcessorValueTreeState.getRawParameterValue("dryWet");

    int delayInSamples = (int)(delayTime * sampleRate);

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        float* delayData = delayBuffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            int delayReadPosition = delayWritePosition - delayInSamples;
            if (delayReadPosition < 0)
                delayReadPosition += delayBufferSamples;

            const float delayedSample = delayData[delayReadPosition];

            float inputSample = channelData[sample];

            delayData[delayWritePosition] = inputSample + (delayedSample * feedback);

            channelData[sample] = inputSample * (1.0f - dryWet) + delayedSample * dryWet;

            delayWritePosition++;
            if (delayWritePosition >= delayBufferSamples)
                delayWritePosition = 0;
        }
    }
}

//==============================================================================
bool DelayEffectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayEffectAudioProcessor::createEditor()
{
    return new DelayEffectAudioProcessorEditor (*this);
}

//==============================================================================
void DelayEffectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayEffectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout DelayEffectAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique <juce::AudioParameterFloat>("dryWet", "Dry/Wet", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.1f }, 0.5f));
    return { params.begin(), params.end() };
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayEffectAudioProcessor();
}
