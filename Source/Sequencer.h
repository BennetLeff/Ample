/*
  ==============================================================================

    Sequencer.h
    Created: 24 Sep 2018 8:41:31pm
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include <vector>

#include "SequencerTrack.h"

class SequencerButton;
class SequencerTrack;

class Sequencer : public ChangeBroadcaster,
		          public ChangeListener,
		          public Component,
		          public Thread
{
public:
	/*
	 * Initialize the sequencer with number of steps and tempo.
	 * Set any of the steps at where the Sequencer is constructed.
	*/
	Sequencer(const size_t number_of_steps, const double tempo);
	~Sequencer();
	void clear_trigger(int step_number);
	void update_tempo(double new_tempo);
	void update_trigger(bool on_or_off, int step_number);
	void run() override;
	void resized() override;
	void stop();

	void changeListenerCallback(ChangeBroadcaster* source) override;
	
	uint16_t current_step() { return step_index_; }

	std::vector<bool> steps_{ false };

	static const uint16_t num_sequencer_tracks_ = 2;
	std::array< std::unique_ptr<SequencerTrack>, num_sequencer_tracks_> sequencer_tracks_;
private:
	void step();
	void play();

	double tempo_; // aka BPM
	double sleep_amount_;
	uint16_t step_index_; // Which step are we on in the sequencer.
};