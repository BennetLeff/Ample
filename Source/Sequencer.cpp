/*
  ==============================================================================

    Sequencer.cpp
    Created: 24 Sep 2018 8:41:31pm
    Author:  bennet

  ==============================================================================
*/

#include <memory>
#include <thread>

#include "JuceHeader.h"
#include "Sequencer.h"

void Event::changeListenerCallback(ChangeBroadcaster* source)
{
	auto seq_button = static_cast<SequencerButton*>(source);

	/* For now assume that source is a SequencerButton and toggle on/off. */
	auto step = static_cast<SequencerButton*>(source);

	if (step)
		on_ = step->is_on_;

	// on_ = !on_;
}

Sequencer::Sequencer(const size_t number_of_steps, const double tempo)
	: tempo_(tempo), step_index_(0),
	sleep_amount_(tempo_ > 0 ? 60.0 / tempo_ : 0),
	Thread("Sequencer Thread")
{
	steps_.resize(number_of_steps);
	for (int i = 0; i < number_of_steps; i++)
	{
		clear_trigger(i);
		addChangeListener(&steps_.at(i));
	}

	startThread();
}

Sequencer::~Sequencer()
{
	stopThread(500);
}

void Sequencer::update_tempo(double new_tempo)
{
	tempo_ = new_tempo;
	sleep_amount_ = 60.0 / tempo_;
}

void Sequencer::update_trigger(bool on_or_off, int step_number)
{
	steps_.at(step_number).on_ = on_or_off;
}

void Sequencer::clear_trigger(int step_number)
{
	steps_.at(step_number).on_ = false;
}

void Sequencer::step()
{
	step_index_ = (step_index_ + 1) % steps_.size();
}

void Sequencer::stop()
{
	stopThread(500);
}

void Sequencer::run()
{
	while (!threadShouldExit())
	{
		play();
		sleep(static_cast<int>(sleep_amount_ * 1000));
	}
}

void Sequencer::play()
{	
	/*
	 * If the current event is on, send a message that triggers sound, gui, etc.
	 *	Currently, this should trigger:
	 *	  - Any associated SampleSource to play audio.
	 */
	auto& curr_step = steps_.at(step_index_);
	if (curr_step.on_)
		curr_step.sendChangeMessage();
	
	/*
	 * Send message that sequencer step is updated. This should update Listeners including
	 *  - Each associated SequencerTrack: to update the step colours
	 */
	sendChangeMessage();
	step();
}