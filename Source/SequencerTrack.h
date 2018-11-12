/*
  ==============================================================================

    SequencerTrack.h
    Created: 28 Sep 2018 5:48:36pm
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include <array>
#include <memory>

#include "JuceHeader.h"
#include "SampleSource.h"
#include "Sequencer.h"

#define NUM_SEQUENCER_STEPS 8

class SampleSource;

/* 
 * Small utility class used for SequencerStep buttons on gui.
 * In addition to retaining the information for drawing each button,
 * SequencerButtons also act as Broadcasters which update an Event
 * when they are toggled. This turns an Event on or off.
 * Once an Event is on, it's behaviour can be triggered when
 * the Sequencer step owning the Event is reached.
 */
class SequencerButton : public ChangeBroadcaster, public TextButton
{
public:
	SequencerButton()
		: on_colour_(Colours::yellow), off_colour_(Colours::greenyellow), triggered_colour_(Colours::tomato),
			is_on_(false) { }

	void toggle_color();
	void trigger_sequencer_colour();

	bool is_on_;

private:
	Colour on_colour_;
	Colour off_colour_;
	Colour triggered_colour_;
};

class Sequencer;

/*
 * A SequencerTrack is a small abstraction over an array that represents a row of
 * sequencer steps/button triggers.
 */
class SequencerTrack : public Component
{
public:
	SequencerTrack();

	bool is_step_on(uint32_t step) { return sequencer_buttons_.at(step)->is_on_; }

	/*
	 * position_triggers offsets a SequencerTrack's buttons.
	 */
	void position_triggers(uint32_t y_offset = 0);
	void update_trigger_button_colours(uint32_t step_to_update);

	void update(int step);

	std::array<std::unique_ptr<SequencerButton>, NUM_SEQUENCER_STEPS> sequencer_buttons_;
};
