#pragma once

#define MAIN_COMP_WIDTH 800
#define MAIN_COMP_HEIGHT 600

#include <array>
#include <utility>

#include "JuceHeader.h"

#include "FileList.h"
#include "SampleSource.h"
#include "Sequencer.h"
#include "SequencerTrack.h"

#include "MainScene.h"

class FileListingScene : public Component
{

};

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

	// const String xml_file_path_ = "C:/Users/bennet/Documents/Ample/Resources";
	// std::unique_ptr<FileList> file_listing_; // (xml_file_path_);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
