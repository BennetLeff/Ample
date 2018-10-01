/*
  ==============================================================================

    Sequencer.cpp
    Created: 24 Sep 2018 8:41:31pm
    Author:  bennet

  ==============================================================================
*/

#include <memory>
#include <thread>

#include "JuceHeader.h"
#include "Sequencer.h"

Sequencer::Sequencer(const size_t number_of_steps, const double tempo)
	: tempo_(tempo), step_index_(0),
	sleep_amount_(tempo_ > 0 ? 60.0 / tempo_ : 0),
	Thread("Sequencer Thread")
{
	steps_.resize(number_of_steps);
	for (int i = 0; i < number_of_steps; i++)
		clear_trigger(i);

	startThread();
}

Sequencer::~Sequencer()
{
	stopThread(500);
}

void Sequencer::update_tempo(double new_tempo)
{
	tempo_ = new_tempo;
	sleep_amount_ = 60.0 / tempo_;
}

void Sequencer::update_trigger(bool event, int step_number)
{
	steps_.at(step_number) = event;
}

void Sequencer::clear_trigger(int step_number)
{
	steps_.at(step_number) = false;
}

void Sequencer::step()
{
	step_index_ = (step_index_ + 1) % steps_.size();
}

void Sequencer::stop()
{
	stopThread(500);
}

void Sequencer::run()
{
	while (!threadShouldExit())
	{
		play();
		sleep(static_cast<int>(sleep_amount_ * 1000));
	}
}

void Sequencer::play()
{		
	sendChangeMessage();
	step();
}