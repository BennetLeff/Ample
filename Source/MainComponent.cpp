#include "MainComponent.h"
#include "Sequencer.h"

#include <vector>
#include <algorithm>

MainComponent::MainComponent()
	: state_(PlayState::Stopped)
{
    // specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);

	value_tree_ = create_default_value_tree();

    sequencer_ = std::make_shared<Sequencer>(value_tree_, &undo_manager_, NUM_SEQUENCER_STEPS, 140.0);

	// Set up the main scene
    main_scene = std::make_unique<MainScene>(sequencer_);
    addAndMakeVisible(main_scene.get());
    main_scene->setSize(MAIN_COMP_WIDTH, MAIN_COMP_HEIGHT);
	
	sample_editor_scene_ = std::make_unique<SampleEditorScene>();
    addAndMakeVisible(sample_editor_scene_.get());
    sample_editor_scene_->setSize(MAIN_COMP_WIDTH, MAIN_COMP_HEIGHT);
	
	// Set up the "file listing" scene
    file_listing_scene = std::make_unique<FileListingScene>(xml_file_path_, sequencer_);
    addChildComponent(file_listing_scene.get());
    file_listing_scene->setSize(MAIN_COMP_WIDTH, MAIN_COMP_HEIGHT);

    addKeyListener(this);
	
	// Add each of these SampleSources to the mixer so they can be played.
	for (auto& track : sequencer_->sequencer_tracks_)
		mixer_source_.addInputSource(track->sample_source_.get(), false);

	value_tree_.addListener(this);

	// Make sure you set the size of the component after
	// you add any child components.
	setSize(MAIN_COMP_WIDTH, MAIN_COMP_HEIGHT);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	mixer_source_.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{	
	mixer_source_.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
}

void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
}

bool MainComponent::keyPressed(const KeyPress& key, Component* originating_component)
{
    if (key == key.leftKey)
    {
        main_scene->setVisible(true);
        file_listing_scene->setVisible(false);
        sample_editor_scene_->setVisible(false);
    }

    else if (key == key.rightKey)
    {
        main_scene->setVisible(false);
        sample_editor_scene_->setVisible(false);
        file_listing_scene->setVisible(true);
    }
	
	else if (key == key.upKey)
    {
        main_scene->setVisible(false);
        file_listing_scene->setVisible(false);
        sample_editor_scene_->setVisible(true);
		// Must request focus or the KeyListener will stop listening, only in SampleEditorScene (not sure why).
		setWantsKeyboardFocus(true);
    }

	switch (key.getTextCharacter())
	{
	case '1':
		cur_track = 0;
		break;
	case '2':
		cur_track = 1;
		break;
	case '3':
		cur_track = 2;
		break;
	case '4':
		cur_track = 3;
		break;
	case '5':
		cur_track = 4;
		break;
	case '6':
		cur_track = 5;
		break;
	case '7':
		cur_track = 6;
		break;
	case '8':
		cur_track = 7;
		break;
	default:
		break;
	}

	// Set the current track to sequence
	// Now user qwertyui keys to turn on or off a step on cur track
	switch (key.getKeyCode())
	{
	case 81: // q
		sequencer_->sequencer_tracks_.at(cur_track)->update(0); // flips on/off at 0
		break;
	case 87: // w
		sequencer_->sequencer_tracks_.at(cur_track)->update(1); // flips on/off at 1
		break;
	case 69: // e
		sequencer_->sequencer_tracks_.at(cur_track)->update(2); // flips on/off at 2
		break;
	case 82: // r
		sequencer_->sequencer_tracks_.at(cur_track)->update(3); // flips on/off at 3
		break;
	case 84: // t
		sequencer_->sequencer_tracks_.at(cur_track)->update(4); // flips on/off at 4
		break;
	case 89: // y
		sequencer_->sequencer_tracks_.at(cur_track)->update(5); // flips on/off at 5
		break;
	case 85: // u
		sequencer_->sequencer_tracks_.at(cur_track)->update(6); // flips on/off at 6
		break;
	case 73: // i
		sequencer_->sequencer_tracks_.at(cur_track)->update(7); // flips on/off at 7
		break;
	default:
		break;
	}

    grabKeyboardFocus();

    return true;
}

bool MainComponent::keyStateChanged(bool is_key_down, Component* originating_component)
{

    return true;
}

ValueTree MainComponent::create_default_value_tree()
{
	ValueTree value_tree(IDs::Sequencer);
	// The following line serves as an example, will be changed soon.
	value_tree.setProperty(IDs::AmpleInfo::name, "Ample!", nullptr);

	return value_tree;
}

void MainComponent::valueTreePropertyChanged(ValueTree & modified_tree, const Identifier & property)
{
	if (property == IDs::SampleSourceProps::file_path)
	{
		Logger::writeToLog(property.toString() + " property changed to " + modified_tree.getProperty(property));
		sample_editor_scene_->set_sample(modified_tree[property]);
	}
}
