/*
  ==============================================================================

    Sequencer.h
    Created: 24 Sep 2018 8:41:31pm
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include <functional>
#include <vector>

class Event
{
public:
	Event() :
		callback_([]() {}) { } 
	Event(std::function<void()> callback) : callback_(callback) { }

	void execute() { std::invoke(callback_); }
private:
	std::function<void()> callback_;
};

class Sequencer : public ChangeBroadcaster, public Thread
{
public:
	/*
	 * Initialize the sequencer with number of steps and tempo.
	 * Set any of the steps at where the Sequencer is constructed.
	*/
	Sequencer(const size_t number_of_steps, const double tempo);
	~Sequencer();
	void run() override;
	void stop();
	void update_trigger(bool event, int step_number);
	void clear_trigger(int step_number);
	void update_tempo(double new_tempo);
	uint16_t current_step() { return step_index_; }

private:
	void step();
	void play();

	std::vector<bool> steps_{ false };
	double tempo_; // aka BPM
	double sleep_amount_ = tempo_ > 0 ? 60.0 / tempo_ : 0;
	uint16_t step_index_; // Which step are we on in the sequencer.
};