/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayEffectAudioProcessorEditor::DelayEffectAudioProcessorEditor(DelayEffectAudioProcessor& p)
	: AudioProcessorEditor(&p),
	audioProcessor(p),
	dryWetSliderRelay("dryWet"),
	delayTimeSliderRelay("delayTime"),
	feedbackSliderRelay("feedback"),
	delayTimeTypeComboBoxRelay("delayTimeType"),
	delayTimeFractionSliderRelay("delayTimeFraction"),
	isPingPongToggleButtonRelay("isPingPong"),
	webBrowserComponent(
		juce::WebBrowserComponent::Options{}
		.withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
		.withWinWebView2Options(juce::WebBrowserComponent::Options::WinWebView2{}
			.withUserDataFolder(juce::File::getSpecialLocation(juce::File::SpecialLocationType::tempDirectory)))
		.withNativeIntegrationEnabled()
		.withOptionsFrom(dryWetSliderRelay)
		.withOptionsFrom(delayTimeSliderRelay)
		.withOptionsFrom(feedbackSliderRelay)
		.withOptionsFrom(delayTimeTypeComboBoxRelay)
		.withOptionsFrom(delayTimeFractionSliderRelay)
		.withOptionsFrom(isPingPongToggleButtonRelay)
		.withOptionsFrom(controlParameterIndexReceiver)
	),
	dryWetAttachment(
		*audioProcessor.audioProcessorValueTreeState.getParameter("dryWet"),
		dryWetSliderRelay,
		audioProcessor.audioProcessorValueTreeState.undoManager
	),
	delayTimeAttachment(
		*audioProcessor.audioProcessorValueTreeState.getParameter("delayTime"),
		delayTimeSliderRelay,
		audioProcessor.audioProcessorValueTreeState.undoManager
	),
	feedbackAttachment(
		*audioProcessor.audioProcessorValueTreeState.getParameter("feedback"),
		feedbackSliderRelay,
		audioProcessor.audioProcessorValueTreeState.undoManager
	),
	delayTimeTypeAttachment(
		*audioProcessor.audioProcessorValueTreeState.getParameter("delayTimeType"),
		delayTimeTypeComboBoxRelay,
		audioProcessor.audioProcessorValueTreeState.undoManager
	),
	delayTimeFractionAttachment(
		*audioProcessor.audioProcessorValueTreeState.getParameter("delayTimeFraction"),
		delayTimeFractionSliderRelay,
		audioProcessor.audioProcessorValueTreeState.undoManager
	),
	isPingPongAttachment(
		*audioProcessor.audioProcessorValueTreeState.getParameter("isPingPong"),
		isPingPongToggleButtonRelay,
		audioProcessor.audioProcessorValueTreeState.undoManager
	)
{
	addAndMakeVisible(audioProcessor.waveFormViewer);
	addAndMakeVisible(webBrowserComponent);
	audioProcessor.waveFormViewer.setColours(juce::Colours::black, juce::Colour(214, 214, 214));
	webBrowserComponent.goToURL(LOCAL_DEV_SERVER_ADDRESS);
	setSize(700, 600);
}

DelayEffectAudioProcessorEditor::~DelayEffectAudioProcessorEditor()
{
}

//==============================================================================
void DelayEffectAudioProcessorEditor::paint(juce::Graphics& g)
{
}

void DelayEffectAudioProcessorEditor::resized()
{
	const int waveFormViewerHeight = 150;
	audioProcessor.waveFormViewer.setBounds(0, 0, getLocalBounds().getWidth(), waveFormViewerHeight);
	webBrowserComponent.setBounds(getLocalBounds().withTrimmedTop(waveFormViewerHeight));
}