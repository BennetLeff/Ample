/*
  ==============================================================================

    SequencerTrack.h
    Created: 28 Sep 2018 5:48:36pm
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include <array>
#include <memory>

#include "SampleSource.h"
#include "Sequencer.h"

#define NUM_SEQUENCER_STEPS 8

struct SequencerButton
{
	bool is_on_;
};

/*
 * A SequencerTrack is a small abstraction over an array that represents a row of
 * sequencer steps/button triggers.
 */
class SequencerTrack : public Component
{
public:
	SequencerTrack();

	bool is_step_on(uint32_t step);
	void update(int step);

	std::array<std::unique_ptr<SequencerButton>, NUM_SEQUENCER_STEPS> sequencer_buttons_;
};
