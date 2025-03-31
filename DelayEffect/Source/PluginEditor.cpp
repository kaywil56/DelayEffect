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
		.withResourceProvider([this](const auto& url)
			{
				return getResource(url);
			},
			juce::URL{ LOCAL_DEV_SERVER_ADDRESS }.getOrigin())
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
	webBrowserComponent.goToURL(juce::WebBrowserComponent::getResourceProviderRoot());
	setSize(650, 500);
}

DelayEffectAudioProcessorEditor::~DelayEffectAudioProcessorEditor()
{
}

void DelayEffectAudioProcessorEditor::paint(juce::Graphics& g){}


void DelayEffectAudioProcessorEditor::resized()
{
	const int waveFormViewerHeight = 100;
	audioProcessor.waveFormViewer.setBounds(0, 0, getLocalBounds().getWidth(), waveFormViewerHeight);
	webBrowserComponent.setBounds(getLocalBounds().withTrimmedTop(waveFormViewerHeight));
}

std::optional<juce::WebBrowserComponent::Resource> DelayEffectAudioProcessorEditor::getResource(const juce::String& url)
{
	const auto urlToRetrive = url == "/" ? juce::String{ "index.html" }
	: url.fromFirstOccurrenceOf("/", false, false);

	if (auto* archive = getZipFile())
	{
		if (auto* entry = archive->getEntry(urlToRetrive))
		{
			auto stream = rawToUniquePtr(archive->createStreamForEntry(*entry));
			auto v = streamToVector(*stream);
			auto mime = getMimeForExtension(getExtension(entry->filename).toLowerCase());
			return juce::WebBrowserComponent::Resource{ std::move(v),
												   std::move(mime) };
		}
	}
	
	return std::nullopt;
}

juce::ZipFile* DelayEffectAudioProcessorEditor::getZipFile()
{
	static auto stream = createAssetInputStream("ui.zip");

	if (stream == nullptr)
	{
		return nullptr;
	}

	static juce::ZipFile f{ stream.get(), false };
	return &f;
}

std::unique_ptr<juce::InputStream> DelayEffectAudioProcessorEditor::createAssetInputStream(const char* resourcePath)
{
	auto assetsDir = getParentDirectory();
	auto resourceFile = assetsDir.getChildFile(resourcePath);

	if (!resourceFile.existsAsFile())
	{
		return {};
	}

	return resourceFile.createInputStream();
}

juce::File DelayEffectAudioProcessorEditor::getParentDirectory() noexcept
{
	auto currentFile = juce::File(__FILE__).getParentDirectory().getParentDirectory();
	auto parentDir = currentFile.getParentDirectory();
	return parentDir;
}

const char* DelayEffectAudioProcessorEditor::getMimeForExtension(const juce::String& extension)
{
	static const std::unordered_map<juce::String, const char*> mimeMap =
	{
		{ { "htm"   },  "text/html"                },
		{ { "html"  },  "text/html"                },
		{ { "txt"   },  "text/plain"               },
		{ { "jpg"   },  "image/jpeg"               },
		{ { "jpeg"  },  "image/jpeg"               },
		{ { "svg"   },  "image/svg+xml"            },
		{ { "ico"   },  "image/vnd.microsoft.icon" },
		{ { "json"  },  "application/json"         },
		{ { "png"   },  "image/png"                },
		{ { "css"   },  "text/css"                 },
		{ { "map"   },  "application/json"         },
		{ { "js"    },  "text/javascript"          },
		{ { "woff2" },  "font/woff2"               }
	};

	if (const auto it = mimeMap.find(extension.toLowerCase()); it != mimeMap.end())
		return it->second;

	jassertfalse;
	return "";
}

juce::String DelayEffectAudioProcessorEditor::getExtension(juce::String filename)
{
	return filename.fromLastOccurrenceOf(".", false, false);
}

std::vector<std::byte> DelayEffectAudioProcessorEditor::streamToVector(juce::InputStream& stream)
{
	std::vector<std::byte> result((size_t)stream.getTotalLength());
	stream.setPosition(0);
	const auto bytesRead = stream.read(result.data(), result.size());
	return result;
}