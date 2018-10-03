/*
  ==============================================================================

    Sequencer.h
    Created: 24 Sep 2018 8:41:31pm
    Author:  bennet

  ==============================================================================
*/

#pragma once

// #include <functional>
#include <vector>

#include "SequencerTrack.h"

class SequencerButton;

/*
 * It may at first seem counterintuitive for an object to be a 
 * broadcaster and a listener. However, this is necessary as each
 * Event must be updated when the corresponding SequencerButton is
 * toggled on or off, and they must Broadcast to some number of 
 * behaviours. A typical example is Broadcasting to a SampleSource
 * to play or stop.
 */
//class Event : public ChangeBroadcaster, public ChangeListener
//{
//public:
//	Event() : on_(false) { }
//	Event(const Event& ev) : on_(ev.on_) { }
//
//	void changeListenerCallback(ChangeBroadcaster* source) override;
//
//	bool on_ = false;
//};

class Sequencer : public ChangeBroadcaster, public Thread
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
	void stop();
	
	uint16_t current_step() { return step_index_; }

	std::vector<bool> steps_{ false };
private:
	void step();
	void play();

	double tempo_; // aka BPM
	double sleep_amount_;
	uint16_t step_index_; // Which step are we on in the sequencer.
};