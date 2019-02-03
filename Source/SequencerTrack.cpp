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

SequencerTrack::SequencerTrack()
{
	for (auto& button : sequencer_steps_)
	{
		button = std::make_unique<SequencerStep>();
	}
}

void SequencerTrack::bind_sample(std::shared_ptr<SampleSource> sample)
{
	// Binding a sample must also tell the AudioSource / MixerAudioSource to update since the 
	// sample is changed.
	sample_source_ = sample;
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

