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
#include "ValueTreeObject.h"

class SampleSource;
class SequencerButton;
class SequencerTrack;

class Sequencer : public Thread,
				  public ValueTreeObject<IDs::Sequencer>
{
public:
	/*
	 * Initialize the sequencer with number of steps and tempo.
	 * Set any of the steps at where the Sequencer is constructed.
	*/
	Sequencer(ValueTree& value_tree, UndoManager* undo_manager, const size_t number_of_steps, const double tempo);
	~Sequencer();
	void update_tempo(double new_tempo);
	double get_tempo() { return tempo_; }
	void run() override;
	void stop();
	void bind_sample_to_track(int track, const String& file_path);

	uint32_t current_step_index();

	static const uint32_t num_sequencer_tracks_ = 8;
	std::array< std::unique_ptr<SequencerTrack>, num_sequencer_tracks_> sequencer_tracks_;
private:
	void step();
	void play();

	CachedValue<double> tempo_;
	CachedValue<int> step_index_; // Which step are we on in the sequencer.
	double sleep_amount_;
	uint32_t last_step_;

	ValueTree state_;
};