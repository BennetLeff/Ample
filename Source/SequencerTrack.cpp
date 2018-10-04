/*
  ==============================================================================

    SequencerTrack.cpp
    Created: 28 Sep 2018 5:48:36pm
    Author:  bennet

  ==============================================================================
*/

#include "SequencerTrack.h"

void SequencerButton::attach_sample_source(ChangeListener& sample_source)
{
	addChangeListener(&sample_source);
}

void SequencerButton::toggle()
{
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

SequencerTrack::SequencerTrack(const std::shared_ptr<Sequencer>& main_sequencer)
	: sequencer_(main_sequencer)
{
	for (auto& button : sample_assigners_)
	{
		button = std::make_unique<SequencerButton>();
	
		/*
		 *  When a button is clicked it should toggle its on/off state and change color.
		 */
		button->onClick = [&button] {
			button->is_on_ = !button->is_on_;
			button->toggle();
		};

		addAndMakeVisible(button.get());
	}
}

void SequencerTrack::add_and_make_visible(Component* parent)
{
	for (auto& button : this->sample_assigners_)
		parent->addAndMakeVisible(button.get());
}

void SequencerTrack::attach_sample(ChangeListener& sample_source)
{
	for (auto& button : sample_assigners_)
		button->attach_sample_source(sample_source);
}

void SequencerTrack::changeListenerCallback(ChangeBroadcaster* source)
{
	/*
	 * There needs to be a way to ensure the only broadcaster source
	 * is a sequencer. However, until that is written, we'll assume
	 * the source is the global sequencer.
	 * For reasons, the raw pointer within a weak_ptr can only be 
	 * referenced if a shared_ptr is constructed by a weak_ptr... 
	 * So for now, we lock and get the address to see if the 
	 * sequencer is the source.
	 */
	auto sequencer_source = static_cast<Sequencer*>(source);
	auto cur_step = sequencer_source->current_step();
	if (is_step_on(cur_step))
		sample_assigners_.at(cur_step)->sendChangeMessage();
	update_trigger_button_colours(cur_step);
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
		sample_assigners_.at(step_to_update - 1)->toggle();
	else
		sample_assigners_.at(sample_assigners_.size() - 1)->toggle();

	sample_assigners_.at(step_to_update)->trigger_sequencer_colour();
}

