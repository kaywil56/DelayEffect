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
		.withOutput("Output",juce::AudioChannelSet::stereo())),
	audioProcessorValueTreeState(*this, nullptr, "Params", createParams(parameters)),
	waveFormViewer(1)
{
	audioProcessorValueTreeState.addParameterListener("dryWet", this);
	audioProcessorValueTreeState.addParameterListener("feedback", this);
	audioProcessorValueTreeState.addParameterListener("delayTime", this);
	audioProcessorValueTreeState.addParameterListener("delayTimeType", this);
	audioProcessorValueTreeState.addParameterListener("delayTimeFraction", this);
	audioProcessorValueTreeState.addParameterListener("isPingPong", this);

	waveFormViewer.setRepaintRate(30);
	waveFormViewer.setBufferSize(64);
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

void DelayEffectAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String DelayEffectAudioProcessor::getProgramName(int index)
{
	return {};
}

void DelayEffectAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void DelayEffectAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	processSpec.sampleRate = sampleRate;
	processSpec.maximumBlockSize = samplesPerBlock;
	processSpec.numChannels = getTotalNumOutputChannels();

	delayLine.reset();
	delayLine.setMaximumDelayInSamples(sampleRate * 3.0f);
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

void DelayEffectAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;

	const int numChannels = getTotalNumOutputChannels();
	const int numSamples = buffer.getNumSamples();

	for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
		buffer.clear(i, 0, buffer.getNumSamples());
	
	if (auto bpmFromHost = getPlayHead())
	{
		auto position = bpmFromHost->getPosition();
		if (position && position->getBpm())
		{
			bpm = *position->getBpm();
		}
	}

	float delayTimeWholeNote = (60.0f / bpm) * 4;

	float noteDuration = getNoteDuration((int)delayTimeTypeIndex, delayTimeWholeNote);
	float delayInSamples = noteDuration * processSpec.sampleRate;

		float* leftChannelData = buffer.getWritePointer(0);
		float* rightChannelData = buffer.getWritePointer(1);

		for (int sample = 0; sample < numSamples; ++sample)
		{
			float inputL = leftChannelData[sample];
			float inputR = rightChannelData[sample];

			float delayedL = delayLine.popSample(0, delayInSamples);
			float delayedR = delayLine.popSample(1, delayInSamples);

			float feedbackL = inputL + (delayedR * feedback);
			float feedbackR = inputR + (delayedL * feedback);

			delayLine.pushSample(0, feedbackL);
			delayLine.pushSample(1, feedbackR);

			leftChannelData[sample] = inputL * (1.0f - dryWet) + delayedL * dryWet;
			rightChannelData[sample] = inputR * (1.0f - dryWet) + delayedR * dryWet;
		}

		if (isPingPong == false) 
		{
			buffer.addFrom(0, 0, buffer, 1, 0, buffer.getNumSamples());
			buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
		}
	
	waveFormViewer.pushBuffer(buffer);
}

float DelayEffectAudioProcessor::getNoteDuration(int delayTimeTypeIndex, float delayTimeWholeNote)
{
	switch (delayTimeTypeIndex) {
	case 0: // Seconds
		return delayTime;
	case 1: // Normal
		return delayTimeWholeNote / delayTimeFractionValues[(int)delayTimeFractionIndex];
	case 2: // Triplet
		return (delayTimeWholeNote / delayTimeFractionValues[(int)delayTimeFractionIndex]) * (2.0f / 3.0f);
	case 3: // Dotted
		return (delayTimeWholeNote / delayTimeFractionValues[(int)delayTimeFractionIndex]) * 1.5f;
	default:
		return delayTime;
	}
};

//==============================================================================
bool DelayEffectAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayEffectAudioProcessor::createEditor()
{
	return new DelayEffectAudioProcessorEditor(*this);
}

//==============================================================================
void DelayEffectAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void DelayEffectAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout DelayEffectAudioProcessor::createParams(Parameters& parameters)
{
	juce::AudioProcessorValueTreeState::ParameterLayout layout;

	{
		auto parameter = std::make_unique <juce::AudioParameterFloat>(
			"dryWet",
			"Dry/Wet",
			juce::NormalisableRange<float> { 0.0f, 1.0f, 0.1f },
			0.5f
		);
		parameters.dryWet = parameter.get();
		layout.add(std::move(parameter));
	}
	{
		auto parameter = std::make_unique <juce::AudioParameterFloat>(
			"delayTime",
			"Delay Time",
			juce::NormalisableRange<float> { 0.01f, 2.0f, 0.1f },
			0.2f
		);
		parameters.delayTime = parameter.get();
		layout.add(std::move(parameter));
	}
	{
		auto parameter = std::make_unique <juce::AudioParameterFloat>(
			"delayTimeFraction",
			"Delay Time Fraction",
			juce::NormalisableRange<float> { 0.0f, 4.0f, 1.0f },
			0.0f
		);
		parameters.delayTimeFraction = parameter.get();
		layout.add(std::move(parameter));
	}
	{
		auto parameter = std::make_unique<juce::AudioParameterFloat>(
			"feedback",
			"Feedback",
			juce::NormalisableRange<float> { 0.0f, 1.0f, 0.1f },
			0.5f
		);
		parameters.feedback = parameter.get();
		layout.add(std::move(parameter));
	}
	{
		auto parameter = std::make_unique<juce::AudioParameterChoice>(
			"delayTimeType",
			"Delay Time Type",
			juce::StringArray{ "Sec", "Nrm", "Trip", "Dot" },
			0
		);
		parameters.delayTimeType = parameter.get();
		layout.add(std::move(parameter));
	}
	{
		auto parameter = std::make_unique<juce::AudioParameterBool>(
			"isPingPong",
			"Ping Pong",
			false
		);
		parameters.isPingPong = parameter.get();
		layout.add(std::move(parameter));
	}
	return layout;
}

void DelayEffectAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
	if (parameterID == "dryWet")
	{
		dryWet = newValue;
	}
	else if (parameterID == "delayTime")
	{
		delayTime = newValue;
	}
	else if (parameterID == "delayTimeType")
	{
		delayTimeTypeIndex = newValue;
	}
	else if (parameterID == "delayTimeFraction")
	{
		delayTimeFractionIndex = newValue;
	}
	else if (parameterID == "isPingPong")
	{
		isPingPong = newValue;
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
