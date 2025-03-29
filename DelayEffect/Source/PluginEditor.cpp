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
    )
{
    addAndMakeVisible(audioProcessor.waveFormViewer);
    addAndMakeVisible(webBrowserComponent);
    audioProcessor.waveFormViewer.setColours(juce::Colours::black, juce::Colours::yellow.withAlpha(0.5f));
    webBrowserComponent.goToURL(LOCAL_DEV_SERVER_ADDRESS);
    setSize(700, 600);
}

DelayEffectAudioProcessorEditor::~DelayEffectAudioProcessorEditor()
{
}

//==============================================================================
void DelayEffectAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void DelayEffectAudioProcessorEditor::resized()
{
    audioProcessor.waveFormViewer.setBounds(0, 0, getLocalBounds().getWidth(), 100);
    webBrowserComponent.setBounds(getLocalBounds().withTrimmedTop(100));
}