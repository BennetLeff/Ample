/*
  ==============================================================================

    SequencerTrack.h
    Created: 28 Sep 2018 5:48:36pm
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SampleSource.h"
#include "Sequencer.h"

#include <array>

#define NUM_SEQUENCER_STEPS 8

/* Small utility class only used for drawing buttons on gui. */
class SequencerButton : public TextButton
{
public:
	SequencerButton()
		: on_colour_(Colours::yellow), off_colour_(Colours::greenyellow), triggered_colour_(Colours::tomato) { } 

	SequencerButton(const Colour& on_colour, const Colour& off_colour, const Colour& trigger_colour)
		: on_colour_(on_colour), off_colour_(off_colour), triggered_colour_(trigger_colour) { }

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
	Colour on_colour_;
	Colour off_colour_;
	Colour triggered_colour_;
};

/*
 * A SequencerTrack is a small abstraction over an array that represents a row of
 * sequencer steps/button triggers. 
 */
class SequencerTrack
{
public:
	SequencerTrack();

	bool is_step_on(uint16_t step);
	void position_triggers(uint16_t y_offset = 0);
	void update_trigger_button_colours(uint16_t step_to_update);

	std::array<std::unique_ptr<SequencerButton>, NUM_SEQUENCER_STEPS> sample_assigners_;
};
