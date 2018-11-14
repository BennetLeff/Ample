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
	for (auto& button : sequencer_buttons_)
	{
		button = std::make_unique<SequencerButton>();
	}
}

bool SequencerTrack::is_step_on(uint32_t step)
{
    return sequencer_buttons_.at(step)->is_on_;
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
}