/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SampleSource.h"
#include "Sequencer.h"

#include <array>

#define NUM_SEQUENCER_STEPS 8

/* Small utility class only used for drawing buttons on gui. */
class SequencerButton : public TextButton
{
public:
	void toggle_on_off_colour() 
	{
		if (is_on_)
			setColour(TextButton::buttonColourId, on_colour_);
		else 
			setColour(TextButton::buttonColourId, off_colour_);
	}

	void trigger_sequencer_colour()
	{
		/* Changes the colour when the sequencer step is on this particular button. */
		setColour(TextButton::buttonColourId, triggered_colour_);
	}

	bool is_on_ = false;

private:
	Colour on_colour_ = Colour(Colours::yellow);
	Colour off_colour_ = Colour(Colours::greenyellow);
	Colour triggered_colour_ = Colour(Colours::tomato);
};


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent, public ChangeListener
{
public:
    MainComponent();
    ~MainComponent();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (Graphics& g) override;
    void resized() override;

	void changeListenerCallback(ChangeBroadcaster* source) override;
private:
	enum class PlayState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	} state_;

	TextButton open_button_kick_;
	TextButton open_button_snare_;

	std::array<std::unique_ptr<SequencerButton>, NUM_SEQUENCER_STEPS> sample_assigners_;

	TextButton play_button_;
	TextButton stop_button_;

	SampleSource sampler_source_kick_;
	SampleSource sampler_source_snare_;
	MixerAudioSource mixer_source_;
	Sequencer sequencer_;

	void change_state(PlayState new_state);
	void open_button_kick_clicked(); 
	void open_button_snare_clicked();
	void play_button_clicked();
	void setup_trigger_buttons(std::array<std::unique_ptr<SequencerButton>, NUM_SEQUENCER_STEPS>& button_array);
	void stop_button_clicked();
	void trigger_button_color(uint16_t step_number);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
