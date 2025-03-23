/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayEffectAudioProcessorEditor::DelayEffectAudioProcessorEditor (DelayEffectAudioProcessor& p)
    : AudioProcessorEditor (&p), 
    audioProcessor (p),
   dryWetSliderRelay("dryWet"),
   delayTimeSliderRelay("delayTime"),
   feedbackSliderRelay("feedback"),
    webBrowserComponent(
        juce::WebBrowserComponent::Options{}
        .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
        .withWinWebView2Options(juce::WebBrowserComponent::Options::WinWebView2{}
            .withUserDataFolder(juce::File::getSpecialLocation(juce::File::SpecialLocationType::tempDirectory)))
        .withNativeIntegrationEnabled()
        .withOptionsFrom(dryWetSliderRelay)
        .withOptionsFrom(delayTimeSliderRelay)
        .withOptionsFrom(feedbackSliderRelay)
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
    )
{
    addAndMakeVisible(webBrowserComponent);
    webBrowserComponent.goToURL("localhost:5173");
    setSize(400, 300);
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
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
        // subcomponents in your editor..
    webBrowserComponent.setBounds(getLocalBounds());
}
