#pragma once

#define MAIN_COMP_WIDTH 800
#define MAIN_COMP_HEIGHT 600

#include <array>
#include <utility>

#include "JuceHeader.h"

#include "FileListingScene.h"
#include "MainScene.h"
#include "SampleSource.h"

/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent, 
					  public KeyListener,
					  public ChangeBroadcaster,
					  public ValueTree::Listener
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
	void valueTreePropertyChanged(ValueTree&, const Identifier&) override;
	void valueTreeChildAdded(ValueTree&, ValueTree&) override {}
	void valueTreeChildRemoved(ValueTree&, ValueTree&, int) override {}
	void valueTreeChildOrderChanged(ValueTree&, int, int) override {}
	void valueTreeParentChanged(ValueTree&) override {}

	enum class PlayState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	} state_;

	ValueTree create_default_value_tree();

	// The value tree root is held in the main component.
	// ValueTree should be used to structure the JUCE application.
	// value_tree_ could be loaded from serialized data.
	// Since it's not at the moment, we must create a new one each time.
	ValueTree value_tree_;
	UndoManager undo_manager_;

	/*
	 * Each scene has access to a shared_ptr<Sequencer> so that they can modify some aspect of it.
	 * The FileListingScene modifies which audio file each sequencer step points to i.e. kick1.wav.
	 * The MainScene modifies which steps are triggered.
	 */
	std::unique_ptr<MainScene> main_scene;
	std::unique_ptr<FileListingScene> file_listing_scene;
	std::vector<std::shared_ptr<SampleSource>> sample_sources_;
	std::shared_ptr<Sequencer> sequencer_;

	MixerAudioSource mixer_source_;
	SampleSource sample_ = SampleSource();

	bool playing_sample_ = false;
	int last_step = 0;

	const String xml_file_path_ = "C:\\Users\\bennet\\Documents\\Ample\\Resources";

	// KeyPress vars for controlling the sequencer steps
	int cur_track = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
