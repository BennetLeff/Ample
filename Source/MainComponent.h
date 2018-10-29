#pragma once

#define MAIN_COMP_WIDTH 800
#define MAIN_COMP_HEIGHT 600

#include <array>
#include <utility>

#include "JuceHeader.h"

#include "FileListingScene.h"
#include "MainScene.h"

/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent, public KeyListener
{
public:
    MainComponent();
    ~MainComponent();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (Graphics& g) override;
    void resized() override;

    bool keyPressed(const KeyPress& key, Component* originating_component) override;
    bool keyStateChanged(bool key_is_down, Component* originating_component) override;

private:
	enum class PlayState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	} state_;

	std::unique_ptr<MainScene> main_scene;
	std::unique_ptr<FileListingScene> file_listing_scene;

	const String xml_file_path_ = "~/Documents/Workspace/Ample/Resources";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
