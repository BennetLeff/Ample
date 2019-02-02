/*
  ==============================================================================

    Sequencer.h
    Created: 24 Sep 2018 8:41:31pm
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include <memory>
#include <vector>

#include "SampleSource.h"
#include "SequencerTrack.h"

class SampleSource;
class SequencerButton;
class SequencerTrack;

class Sequencer : public Thread
{
public:
	/*
	 * Initialize the sequencer with number of steps and tempo.
	 * Set any of the steps at where the Sequencer is constructed.
	*/
	Sequencer(const size_t number_of_steps, const double tempo);
	~Sequencer();
	void update_tempo(double new_tempo);
	double get_tempo() { return tempo_; }
	void run() override;
	void stop();
	void bind_sample_to_track(int track, std::shared_ptr<SampleSource> sample_to_bind);

	uint32_t current_step_index();

	static const uint32_t num_sequencer_tracks_ = 2;
	std::array< std::unique_ptr<SequencerTrack>, num_sequencer_tracks_> sequencer_tracks_;
private:
	void step();
	void play();

	double tempo_; // aka BPM
	double sleep_amount_;
	uint32_t step_index_ = 0; // Which step are we on in the sequencer.
	uint32_t last_step_ = 0;
};