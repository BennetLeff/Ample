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
    for (auto& track : sequencer_tracks_)
    {
        track = std::make_unique<SequencerTrack>();
    }

	startThread();
}

Sequencer::~Sequencer()
{
	stopThread(500);
}

uint32_t Sequencer::current_step()
{
    return step_index_;
}

void Sequencer::step()
{
	step_index_ = (step_index_ + 1) % NUM_SEQUENCER_STEPS;
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
		step();
		sleep(static_cast<int>(sleep_amount_ * 1000));
	}
}

void Sequencer::play()
{	
	/*
	 * Send message that sequencer step is updated. This should update Listeners including
	 *  - Each associated SequencerTrack: to update the step colours
	 */
	for (auto& seq_track : sequencer_tracks_)
    {
	    seq_track->update(current_step());
    }

}
