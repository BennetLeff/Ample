#include "MainComponent.h"
#include "Sequencer.h"

#include <vector>
#include <algorithm>

MainComponent::MainComponent()
	: state_(PlayState::Stopped),
	sequencer_(8, 120.0)
{
    // specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);

	addAndMakeVisible(&open_button_kick_);
	open_button_kick_.setButtonText("Open Kick...");
	// this data should be passed to the sampler_source but HOW!?
	open_button_kick_.onClick = [this] { open_button_kick_clicked(); };
	
	addAndMakeVisible(&open_button_snare_);
	open_button_snare_.setButtonText("Open Snare...");
	// this data should be passed to the sampler_source but HOW!?
	open_button_snare_.onClick = [this] { open_button_snare_clicked(); };
	
	/* Define sample assignment buttons. */
	for (auto& button : grid_row_kick_.sample_assigners_)
		addAndMakeVisible(button.get());
	
	for (auto& button : grid_row_snare_.sample_assigners_)
		addAndMakeVisible(button.get());

	addAndMakeVisible(&play_button_);
	play_button_.setButtonText("Play");
	play_button_.onClick = [this] { play_button_clicked(); };
	play_button_.setColour(TextButton::buttonColourId, Colours::green);
	play_button_.setEnabled(false);

	addAndMakeVisible(&stop_button_);
	stop_button_.setButtonText("Stop");
	stop_button_.onClick = [this] { stop_button_clicked(); };
	stop_button_.setColour(TextButton::buttonColourId, Colours::red);
	stop_button_.setEnabled(false);
	
	sampler_source_kick_.addChangeListener(this);
	sampler_source_snare_.addChangeListener(this);
	sequencer_.addChangeListener(this);

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
	sampler_source_kick_.prepareToPlay(samplesPerBlockExpected, sampleRate);
	sampler_source_snare_.prepareToPlay(samplesPerBlockExpected, sampleRate);
	mixer_source_.prepareToPlay(samplesPerBlockExpected, sampleRate);

	mixer_source_.addInputSource(&sampler_source_kick_, false);
	mixer_source_.addInputSource(&sampler_source_snare_, false);
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
	sampler_source_kick_.releaseResources();
	sampler_source_snare_.releaseResources();
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

	grid_row_kick_.position_triggers();
	grid_row_snare_.position_triggers(60);
}

void MainComponent::changeListenerCallback(ChangeBroadcaster * source)
{
	if (source == &sampler_source_kick_)
	{
		if (sampler_source_kick_.is_playing())
			change_state(PlayState::Playing);
		else
			change_state(PlayState::Stopped);
	}

	else if (source == &sequencer_)
	{
		uint16_t cur_step = sequencer_.current_step();
		if (grid_row_kick_.is_step_on(cur_step))
		{
			sampler_source_kick_.start();
		}
		if (grid_row_snare_.is_step_on(cur_step))
		{
			sampler_source_snare_.start();
		}
		else
		{
			sampler_source_kick_.stop();
			sampler_source_snare_.stop();
		}

		/* Handle button drawing here... */
		grid_row_kick_.update_trigger_button_colours(sequencer_.current_step());
		grid_row_snare_.update_trigger_button_colours(sequencer_.current_step());
	}
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
			sampler_source_kick_.set_position(0.0);
			sampler_source_kick_.set_playing(false);
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
			sampler_source_kick_.stop();
			break;
		}
	}
}

void MainComponent::open_button_kick_clicked()
{
	FileChooser chooser("Select a WAV file to play... ",
		File::nonexistent,
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
		File::nonexistent,
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

void MainComponent::stop_button_clicked()
{
	change_state(PlayState::Stopping);
}
