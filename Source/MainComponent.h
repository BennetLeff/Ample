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

struct SequencerGridRow
{
public:
	SequencerGridRow()
	{
		for (auto& button : sample_assigners_)
		{
			button = std::make_unique<SequencerButton>();
			// addAndMakeVisible(button.get());
			button->setColour(TextButton::buttonColourId, Colours::greenyellow);
			button->is_on_ = false;

			button->onClick = [&button] {
				button->is_on_ = !button->is_on_;
				button->toggle_on_off_colour();
			};
		}
	}

	void update_trigger_button_colours(uint16_t step_to_update)
	{
		if (step_to_update != 0)
			sample_assigners_.at(step_to_update - 1)->toggle_on_off_colour();
		else
			sample_assigners_.at(sample_assigners_.size() - 1)->toggle_on_off_colour();

		sample_assigners_.at(step_to_update)->trigger_sequencer_colour();
	}

	void position_triggers(uint16_t y_offset = 0)
	{
		int i = 200;
		for (auto& button : sample_assigners_)
		{
			button->setBounds(i, 200 + y_offset, 40, 40);
			i += 50;
		}
	}

	std::array<std::unique_ptr<SequencerButton>, NUM_SEQUENCER_STEPS> sample_assigners_;
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
	TextButton play_button_;
	TextButton stop_button_;

	MixerAudioSource mixer_source_;
	SampleSource sampler_source_kick_;
	SampleSource sampler_source_snare_;
	Sequencer sequencer_;

	std::array<std::unique_ptr<SequencerButton>, NUM_SEQUENCER_STEPS> sample_assigners_;
	SequencerGridRow grid_row_{};
	SequencerGridRow grid_row_snare_{};


	void change_state(PlayState new_state);
	void open_button_kick_clicked(); 
	void open_button_snare_clicked();
	void play_button_clicked();
	// void initialize_trigger_buttons_array(std::array<std::unique_ptr<SequencerButton>, NUM_SEQUENCER_STEPS>& button_array);
	void stop_button_clicked();
	// void trigger_button_color(uint16_t step_number);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
