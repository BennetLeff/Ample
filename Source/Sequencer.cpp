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

Sequencer::Sequencer(ValueTree& value_tree, UndoManager* undo_manager, const size_t number_of_steps, const double tempo)
	: ValueTreeObject(value_tree, undo_manager),
	step_index_(0),
	sleep_amount_(tempo_ > 0 ? 60.0 / tempo_ : 0),
	tempo_(get_state(), IDs::SequencerProps::tempo, get_undo_manager(), tempo),
	state_(value_tree),
	Thread("Sequencer Thread")
{
    for (auto& track : sequencer_tracks_)
    {
        track = std::make_unique<SequencerTrack>(
								get_state().getOrCreateChildWithName(IDs::SequencerTrack, nullptr),
								undo_manager);
    }

	startThread();
}

uint32_t Sequencer::current_step_index()
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

void Sequencer::bind_sample_to_track(int track, std::shared_ptr<SampleSource> sample_to_bind)
{
	sequencer_tracks_.at(track)->bind_sample(sample_to_bind);
}

void Sequencer::run()
{
	while (!threadShouldExit())
	{
		play();
		Logger::writeToLog("Step " + String(current_step_index()) + " :: " + String());
		step();
		sleep(static_cast<int>(sleep_amount_ * 1000));
	}
}

void Sequencer::update_tempo(double tempo)
{
	tempo_ = tempo;
	sleep_amount_ = tempo_ > 0 ? 60.0 / tempo_ : 0;
}

void Sequencer::play()
{	
	// Loop over every SequencerTrack in the Sequencer


	// If the step has changed since the last time play was called
	// we can call start
	if (last_step_ != current_step_index())
	{

		for (auto& seq_track : sequencer_tracks_)
		{
			// Get the current step
			// This look up should be made more efficient
			auto& cur_step = seq_track->sequencer_steps_.at(current_step_index());
			if (cur_step->is_on_)
				seq_track->sample_source_->start();  //->start();
		}
	}

	last_step_ = this->current_step_index();

}

Sequencer::~Sequencer()
{
    stopThread(500);
}