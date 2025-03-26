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
    audioProcessorValueTreeState.addParameterListener("dryWet", this);
    audioProcessorValueTreeState.addParameterListener("feedback", this);
    audioProcessorValueTreeState.addParameterListener("delayTime", this);
    audioProcessorValueTreeState.addParameterListener("delayTimeType", this);
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
    processSpec.sampleRate = sampleRate;
    processSpec.maximumBlockSize = samplesPerBlock;
    processSpec.numChannels = getTotalNumOutputChannels();

    delayLine.reset();
    delayLine.setMaximumDelayInSamples(sampleRate * 2.0f);
    delayLine.prepare(processSpec);
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

    return;

    const int numChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();

   if (auto bpmFromHost = *getPlayHead()->getPosition()->getBpm())
        bpm = bpmFromHost;

    float delayTimeBeat = 60.0f / bpm;

    float delayInSamples = delayTime * processSpec.sampleRate;

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float inputSample = channelData[sample];
            float delayedSample = delayLine.popSample(channel, delayInSamples);
            float delayedInput = inputSample + (delayedSample * feedback);
            delayLine.pushSample(channel, delayedInput);
            channelData[sample] = inputSample * (1.0f - dryWet) + delayedSample * dryWet;
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
    params.push_back(std::make_unique <juce::AudioParameterFloat>("delayTime", "Delay Time", juce::NormalisableRange<float> { 0.01f, 2.0f, 0.1f }, 0.2f));
    params.push_back(std::make_unique <juce::AudioParameterFloat>("feedback", "Feedback", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.1f }, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("delayTimeType", "Delay Time Type", juce::StringArray{ "SEC", "BEAT", "TRIP", "DOT" }, 0));
    return { params.begin(), params.end() };
}

void DelayEffectAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if(parameterID == "dryWet")
    {
        dryWet = newValue;
    }
    else if (parameterID == "delayTime")
    {
        delayTime = newValue;
    }
    else if (parameterID == "delayTimeType") 
    {
        delayTimeType = [this, newValue]
            {
                switch ((int)newValue)
                {
                case 0:
                    return "SEC";
                case 1:
                    return "BEAT";
                case 2:
                    return "TRIP";
                case 3:
                    return "DOT";
                default:
                    return "SEC";
                }
            }();
        DBG(delayTimeType);
    }
    else 
    {
        feedback = newValue;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayEffectAudioProcessor();
}
