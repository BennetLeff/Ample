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

SequencerTrack::SequencerTrack(const ValueTree& value_tree, UndoManager* undo_manager)
	: ValueTreeObject(value_tree, undo_manager),
	  state_(value_tree)
{
	sample_source_ = std::make_unique<SampleSource>(state_.getOrCreateChildWithName(IDs::SampleSource, undo_manager),
													undo_manager);
	for (auto& button : sequencer_steps_)
		button = std::make_unique<SequencerStep>();
}

void SequencerTrack::bind_sample(const String& file_path)
{
	// Binding a sample must also tell the AudioSource / MixerAudioSource to update since the 
	// sample is changed.
	sample_source_->set_file_path(file_path);
}

bool SequencerTrack::is_step_on(uint32_t step)
{
    return sequencer_steps_.at(step)->is_on_; 
}

void SequencerTrack::update(int step)
{
	// flips a step
	sequencer_steps_.at(step)->is_on_ = !sequencer_steps_.at(step)->is_on_;
}
