#include "MainComponent.h"
#include "Sequencer.h"

#include <vector>
#include <algorithm>

MainComponent::MainComponent()
	: state_(PlayState::Stopped)
{
    // specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);

	/*
	 * Initialize Sequencer and follow by initializing all SequencerTrack objects
	 * with the sequencer_. 
	 * Internally this creates a std::weak_ptr to sequencer_ inside each SequencerTrack.
	 * The sequencer_ starts stepping automatically as its insantiation 
	 * starts a new thread (by calling the overriden run() method).
	 */
	sequencer_ = std::make_shared<Sequencer>(NUM_SEQUENCER_STEPS, 140.0);

	/*
	 * Start adding in GUI components - this should become more programmatic.
	 */
	for (auto& seq_track : sequencer_tracks_)
	{
		seq_track = std::make_unique<SequencerTrack>(sequencer_);
		sequencer_->addChangeListener(seq_track.get());

		//addAndMakeVisible(seq_track.get());
		//seq_track->add_and_make_visible();
	}

	sequencer_tracks_.at(0)->attach_sample(sampler_source_kick_);
	sequencer_tracks_.at(1)->attach_sample(sampler_source_snare_);

	//addAndMakeVisible(&open_button_kick_);
	open_button_kick_.setButtonText("Open Kick...");
	open_button_kick_.onClick = [this] { open_button_kick_clicked(); };
	
	//addAndMakeVisible(&open_button_snare_);
	open_button_snare_.setButtonText("Open Snare...");
	open_button_snare_.onClick = [this] { open_button_snare_clicked(); };

	setup_text_button(play_button_, [this] { play_button_clicked(); }, "Play", Colours::green, false);
	setup_text_button(stop_button_, [this] { stop_button_clicked(); }, "Stop", Colours::red, false);

	mixer_source_.addInputSource(&sampler_source_kick_, false);
	mixer_source_.addInputSource(&sampler_source_snare_, false);


	file_listing_ = std::make_unique<FileList>(xml_file_path_);
	addAndMakeVisible(file_listing_.get());

	// Make sure you set the size of the component after
	// you add any child components.
	setSize(800, 600);
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
	if (sampler_source_kick_.is_empty() || sampler_source_snare_.is_empty())
	{
		bufferToFill.clearActiveBufferRegion();
		return;
	}

	mixer_source_.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
	mixer_source_.releaseResources();
}

void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	open_button_kick_.setBounds(10, 10, getWidth() - 20, 20);
	open_button_snare_.setBounds(10, 40, getWidth() - 20, 20);

	play_button_.setBounds(10, 70, getWidth() - 20, 20);
	stop_button_.setBounds(10, 100, getWidth() - 20, 20);

	for (int i = 0; i < sequencer_tracks_.size(); i++)
		sequencer_tracks_.at(i)->position_triggers( i * 60);

	file_listing_->setBounds(getLocalBounds());
}

void MainComponent::change_state(PlayState new_state)
{
	if (state_ != new_state)
	{
		state_ = new_state;
		switch (state_)
		{
		case PlayState::Stopped:                          
			stop_button_.setEnabled(false);
			play_button_.setEnabled(true);
			break;
		case PlayState::Starting:                          
			play_button_.setEnabled(false);
			stop_button_.setEnabled(true);
			break;
		case PlayState::Playing:                           
			stop_button_.setEnabled(true);
			break;
		case PlayState::Stopping:                          
			play_button_.setEnabled(true);
			break;
		}
	}
}

void MainComponent::open_button_kick_clicked()
{
	FileChooser chooser("Select a WAV file to play... ",
		File(),
		"*.wav");

	if (chooser.browseForFileToOpen())
	{
		auto file = chooser.getResult();
		auto path = file.getFullPathName();
		sampler_source_kick_.set_file_path(path);
		sampler_source_kick_.notify();
	}

	// This should not be stopping but for now it will do.
	change_state(PlayState::Stopping);
	sampler_source_kick_.set_playing(false);
}

void MainComponent::open_button_snare_clicked()
{
	FileChooser chooser("Select a WAV file to play... ",
		File(),
		"*.wav");

	if (chooser.browseForFileToOpen())
	{
		auto file = chooser.getResult();
		auto path = file.getFullPathName();
		sampler_source_snare_.set_file_path(path);
		sampler_source_snare_.notify();
	}

	// This should not be stopping but for now it will do.
	change_state(PlayState::Stopping);
	sampler_source_snare_.set_playing(false);
}

void MainComponent::play_button_clicked()
{
	change_state(PlayState::Starting);
}

void MainComponent::setup_text_button(TextButton& button, std::function<void()> on_click, const String& text, const Colour& colour, const bool on_or_off)
{
	// addAndMakeVisible(button);
	button.onClick = on_click;
	button.setButtonText(text);
	button.setColour(TextButton::buttonColourId, colour);
	button.setEnabled(on_or_off);
}

void MainComponent::stop_button_clicked()
{
	change_state(PlayState::Stopping);
}
