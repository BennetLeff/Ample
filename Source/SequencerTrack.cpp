/*
  ==============================================================================

    SequencerTrack.cpp
    Created: 28 Sep 2018 5:48:36pm
    Author:  bennet

  ==============================================================================
*/

#include "SequencerTrack.h"

SequencerTrack::SequencerTrack()
{
	for (auto& button : sample_assigners_)
	{
		button = std::make_unique<SequencerButton>();
		button->setColour(TextButton::buttonColourId, Colours::greenyellow);
		button->is_on_ = false;
	
		/*
		 *  When a button is clicked it should toggle its on/off state and change color.
		 */
		button->onClick = [&button] {
			button->is_on_ = !button->is_on_;
			button->toggle_on_off_colour();
		};
	}
}

bool SequencerTrack::is_step_on(uint16_t step)
{
	return sample_assigners_.at(step)->is_on_;
}

void SequencerTrack::position_triggers(uint16_t y_offset)
{
	int i = 200;
	for (auto& button : sample_assigners_)
	{
		button->setBounds(i, 200 + y_offset, 40, 40);
		i += 50;
	}
}

void SequencerTrack::update_trigger_button_colours(uint16_t step_to_update)
{
	if (step_to_update != 0)
		sample_assigners_.at(step_to_update - 1)->toggle_on_off_colour();
	else
		sample_assigners_.at(sample_assigners_.size() - 1)->toggle_on_off_colour();

	sample_assigners_.at(step_to_update)->trigger_sequencer_colour();
}

