#include "MainComponent.h"


MainComponent::MainComponent()
	: state_(TransportState::Stopped)
{
    // specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);

	addAndMakeVisible(&open_button_);
	open_button_.setButtonText("Open...");
	// this data should be passed to the sampler_source but HOW!?
	open_button_.onClick = [this] { open_button_clicked(); };
	
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
	
	sampler_source_.addChangeListener(this);

	// Make sure you set the size of the component after
	// you add any child components.
	setSize(800, 600);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
	// Setting current_buffer_ to nullptr is all the clean up we need.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	sampler_source_.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	if (sampler_source_.is_empty())
	{
		bufferToFill.clearActiveBufferRegion();
		return;
	}

	sampler_source_.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
	sampler_source_.releaseResources();
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
	open_button_.setBounds(10, 10, getWidth() - 20, 20);
	play_button_.setBounds(10, 40, getWidth() - 20, 20);
	stop_button_.setBounds(10, 70, getWidth() - 20, 20);
}

void MainComponent::changeListenerCallback(ChangeBroadcaster * source)
{
	if (source == &sampler_source_)
	{
		if (sampler_source_.is_playing())
			change_state(TransportState::Playing);
		else
			change_state(TransportState::Stopped);
	}
}

void MainComponent::change_state(TransportState new_state)
{
	if (state_ != new_state)
	{
		state_ = new_state;
		switch (state_)
		{
		case TransportState::Stopped:                          
			stop_button_.setEnabled(false);
			play_button_.setEnabled(true);
			sampler_source_.set_position(0.0);
			sampler_source_.set_playing(false);
			break;
		case TransportState::Starting:                          
			play_button_.setEnabled(false);
			sampler_source_.start();
			sampler_source_.set_playing(true);
			stop_button_.setEnabled(true);
			break;
		case TransportState::Playing:                           
			stop_button_.setEnabled(true);
			sampler_source_.set_playing(true);
			break;
		case TransportState::Stopping:                          
			sampler_source_.stop();
			sampler_source_.set_playing(false);
			play_button_.setEnabled(true);
			break;
		}
	}
}

void MainComponent::play_button_clicked()
{
	change_state(TransportState::Starting);
}

void MainComponent::stop_button_clicked()
{
	change_state(TransportState::Stopping);
}

void MainComponent::open_button_clicked()
{
	FileChooser chooser("Select a WAV file to play... ",
		File::nonexistent,
		"*.wav");

	if (chooser.browseForFileToOpen())
	{
		auto file = chooser.getResult();
		auto path = file.getFullPathName();
		sampler_source_.chosen_path_.swapWith(path);
		sampler_source_.notify();
	}

	// This should not be stopping but for now it will do.
	change_state(TransportState::Stopping);
	sampler_source_.set_playing(false);
}