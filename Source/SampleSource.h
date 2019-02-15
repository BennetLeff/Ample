/*
  ==============================================================================

    SampleSource.h
    Created: 19 Sep 2018 4:58:00pm
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include <memory>

#include "JuceHeader.h"

#include "Sequencer.h"
#include "ValueTreeObject.h"

struct SampleBuffer
{
	std::unique_ptr<AudioSampleBuffer> buffer_;
	uint32_t position_ = 0;
};

class SampleSource : public AudioTransportSource,
					 public ValueTreeObject<IDs::SampleSource>,
					 public Thread 
{
public:
	SampleSource(const ValueTree& value_tree, UndoManager* undo_manager);
	~SampleSource();
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	void start();
	void stop();

	void set_file_path(const String& path_to_swap);
	void set_size(int num_channels, int num_samples);
	void set_position(uint32_t pos);
	void set_start_position(const juce::uint32 new_start_pos);
	void set_end_position(const juce::uint32 new_end_pos);
	const juce::uint32 get_start_position() const;
	const juce::uint32 get_end_position() const;
	void set_playing(bool playing) { is_playing_ = playing;  }
	bool is_playing() { return is_playing_;  }
	bool is_empty();

	// The directory where files are located. Hardcoded for now.
	CachedValue<String> resources_directory_; 

private:
	// run() is overriden to declare what to do in a new thread
	void run() override;
	void check_for_path_to_open();

	SampleBuffer current_buffer_;

	bool is_playing_;
	int position_;
	juce::uint32 start_position_;
	juce::uint32 end_position_;

	AudioFormatManager format_manager_;

	String file_path_;
	ValueTree state_;
};