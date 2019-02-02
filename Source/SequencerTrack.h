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

class SampleSource;

struct SequencerStep
{
	bool is_on_;
};

/*
 * A SequencerTrack is a small abstraction over an array that represents a row of
 * sequencer steps/button triggers. It contains a SampleSource so that any step
 * that the application can play the sample when a step is on.
 */
class SequencerTrack : public Component
{
public:
	SequencerTrack();

	bool is_step_on(uint32_t step);
	void update(int step);

	void bind_sample(std::shared_ptr<SampleSource> sample);

	// The sample each step will trigger.
	std::shared_ptr<SampleSource> sample_source_;

	std::array<std::unique_ptr<SequencerStep>, NUM_SEQUENCER_STEPS> sequencer_steps_;
};
