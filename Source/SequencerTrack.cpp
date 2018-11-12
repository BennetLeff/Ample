/*
  ==============================================================================

    SequencerTrack.cpp
    Created: 28 Sep 2018 5:48:36pm
    Author:  bennet

  ==============================================================================
*/

#include <algorithm>

#include "SequencerTrack.h"
#include "FileList.h"

void SequencerButton::toggle_color()
{
	MessageManagerLock mm_lock;
	if (is_on_)
		setColour(TextButton::buttonColourId, on_colour_);
	else
		setColour(TextButton::buttonColourId, off_colour_);
}

void SequencerButton::trigger_sequencer_colour()
{
	MessageManagerLock mm_lock;
	/* Changes the colour when the sequencer step is on this particular button. */
	setColour(TextButton::buttonColourId, triggered_colour_);
}

SequencerTrack::SequencerTrack()
{
    auto count = 0;
	for (auto& button : sequencer_buttons_)
	{
		button = std::make_unique<SequencerButton>();
	
		/*
		 *  When a button is clicked it should toggle its on/off state and change color.
		 */
		button->onClick = [&button] {
			button->is_on_ = !button->is_on_;
			button->toggle_color();
		};

		addAndMakeVisible(button.get());
		count += 1;
	}
}

void SequencerTrack::update(int step)
{
    /*
     * Should update by passing a SequencerButton reference but I will fix soon.
     */
	if (sequencer_buttons_.at(step)->is_on_)
	{
		// play the sound
	}
	update_trigger_button_colours(step);
}

void SequencerTrack::position_triggers(uint32_t y_offset)
{
	int i = 200;
	std::for_each(sequencer_buttons_.begin(), sequencer_buttons_.end(),
		[i, y_offset](auto& button) mutable { button->setBounds(i, 40 + y_offset, 40, 40); i += 50; });
}


void SequencerTrack::update_trigger_button_colours(uint32_t step_to_update)
{
	/*
	 * Need to lock since we are drawing out of the gui.
	 */
	if (step_to_update != 0)
		sequencer_buttons_.at(step_to_update - 1)->toggle_color();
	else
		sequencer_buttons_.at(sequencer_buttons_.size() - 1)->toggle_color();

	sequencer_buttons_.at(step_to_update)->trigger_sequencer_colour();
}

