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


bool SampleSourceList::is_suitable_type(const ValueTree& v) const 
{
	return v.hasType(IDs::SampleSource);
}

SampleSource* SampleSourceList::create_new_object(const ValueTree& v) 
{
	SampleSource* ss = new SampleSource(v, nullptr);
	return ss;
}

void SampleSourceList::delete_object(SampleSource* ss) 
{
	delete ss;
}

void SampleSourceList::new_object_added(SampleSource*) { }
void SampleSourceList::object_removed(SampleSource*) { }
void SampleSourceList::object_order_changed() { }


SequencerTrack::SequencerTrack(const ValueTree& value_tree, UndoManager* undo_manager)
	: ValueTreeObject(value_tree, undo_manager),
	  state_(value_tree)
{
	for (auto& button : sequencer_steps_)
	{
		button = std::make_unique<SequencerStep>();
	}

	state_.addChild(ValueTree(IDs::SampleSource), -1, undo_manager);

	SampleSourceList ss_list(state_);

	for (auto ss : state_)
	{
		if (ss.hasType(IDs::SampleSource))
		{
			Logger::writeToLog("Got Sample Source!");
		}
	}
}

void SequencerTrack::bind_sample(const String& file_path)
{
	// Binding a sample must also tell the AudioSource / MixerAudioSource to update since the 
	// sample is changed.
	sample_source_->set_file_path(file_path);
}

bool SequencerTrack::is_step_on(uint32_t step)
{
    return sequencer_steps_.at(step)->is_on_; 
}

void SequencerTrack::update(int step)
{
	// flips a step
	sequencer_steps_.at(step)->is_on_ = !sequencer_steps_.at(step)->is_on_;
}

