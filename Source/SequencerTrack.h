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
	std::shared_ptr<SampleSource> sample_source_;
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

	void bind_sample(std::shared_ptr<SampleSource> sample);

	std::array<std::unique_ptr<SequencerStep>, NUM_SEQUENCER_STEPS> sequencer_steps_;
};
