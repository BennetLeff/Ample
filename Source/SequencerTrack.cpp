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

void SequencerButton::attach_sample_source(ChangeListener& sample_source)
{
	addChangeListener(&sample_source);
}

void SequencerButton::toggle()
{
    is_on_ = !is_on_; // Flip the logical value from true to false or false to true.
	if (is_on_)
		setColour(TextButton::buttonColourId, on_colour_);
	else
		setColour(TextButton::buttonColourId, off_colour_);
}

void SequencerButton::trigger_sequencer_colour()
{
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
			button->toggle();
		};

		button->setBounds(count * 50, 200, 40, 40);
		addAndMakeVisible(button.get());
		count += 1;
	}
}

void SequencerTrack::add_and_make_visible()
{
	// auto parent = getParentComponent();
	std::for_each(sequencer_buttons_.begin(), sequencer_buttons_.end(),
		[this](auto& button) { addAndMakeVisible(button.get()); });
}

void SequencerTrack::attach_sample(ChangeListener& sample_source)
{
	std::for_each(sequencer_buttons_.begin(), sequencer_buttons_.end(),
		[&sample_source](auto& button) { button->attach_sample_source(sample_source); });
}

void SequencerTrack::changeListenerCallback(ChangeBroadcaster* source)
{
    if(auto sequencer_source = dynamic_cast<Sequencer*>(source); sequencer_source != nullptr)
    {
        auto cur_step = sequencer_source->current_step();

        if (is_step_on(cur_step))
            sequencer_buttons_.at(cur_step)->sendChangeMessage();
        update_trigger_button_colours(cur_step);
    }
}

void SequencerTrack::resized()
{
    setBounds(getBoundsInParent());
}

void SequencerTrack::update(int sequencer_button_index)
{
    /*
     * Should update by passing a SequencerButton reference but I will fix soon.
     */
     // if (is_step_on(sequencer_button_index))
     //    sequencer_buttons_.at(sequencer_button_index)->sendChangeMessage();
     // update_trigger_button_colours(sequencer_button_index);
}

bool SequencerTrack::is_step_on(uint16_t step)
{
    return sequencer_buttons_.at(step)->is_on_;
}

void SequencerTrack::position_triggers(uint16_t y_offset)
{
	int i = 200;
	std::for_each(sequencer_buttons_.begin(), sequencer_buttons_.end(),
		[i, y_offset](auto& button) mutable { button->setBounds(i, 200 + y_offset, 40, 40); i += 50; });
}


void SequencerTrack::update_trigger_button_colours(uint16_t step_to_update)
{
	if (step_to_update != 0)
		sequencer_buttons_.at(step_to_update - 1)->toggle();
	else
		sequencer_buttons_.at(sequencer_buttons_.size() - 1)->toggle();

	sequencer_buttons_.at(step_to_update)->trigger_sequencer_colour();
}

