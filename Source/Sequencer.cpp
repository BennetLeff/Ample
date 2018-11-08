/*
  ==============================================================================

    Sequencer.cpp
    Created: 24 Sep 2018 8:41:31pm
    Author:  bennet

  ==============================================================================
*/

#include <algorithm>
#include <memory>
#include <thread>

#include "JuceHeader.h"
#include "Sequencer.h"
#include "SequencerTrack.h"

Sequencer::Sequencer(const size_t number_of_steps, const double tempo)
	: tempo_(tempo), step_index_(0),
	sleep_amount_(tempo_ > 0 ? 60.0 / tempo_ : 0),
	Thread("Sequencer Thread")
{
	steps_.resize(number_of_steps);

	for (auto& track: sequencer_tracks_)
	    track = std::make_unique<SequencerTrack>();

	startThread();

    const MessageManagerLock mm_lock_(this);

    auto count = 0;
    for (auto& track : sequencer_tracks_)
    {
        track->setBounds(0, count * 60, getParentWidth(), getParentHeight());
        addAndMakeVisible(track.get());
        count += 1;
    }
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
	steps_.at(step_number) = on_or_off;
}

void Sequencer::clear_trigger(int step_number)
{
	steps_.at(step_number) = false;
}

void Sequencer::step()
{
	step_index_ = (step_index_ + 1) % steps_.size();
}

void Sequencer::stop()
{
	stopThread(500);
}

void Sequencer::resized()
{
	auto count = 0;
	for (auto& track : sequencer_tracks_)
	{
		track->position_triggers(count * 60);
		count += 1;
	}
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
	 * Send message that sequencer step is updated. This should update Listeners including
	 *  - Each associated SequencerTrack: to update the step colours
	 */
	// sendChangeMessage();
	for (int i = 0; i < num_sequencer_tracks_; i++)
    {
	    sequencer_tracks_.at(i)->update(i);
    }

	step();
}

void Sequencer::changeListenerCallback(ChangeBroadcaster *source)
{

}