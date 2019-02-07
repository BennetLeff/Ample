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
					 public ChangeListener,
					 public ValueTreeObject<IDs::SampleSource>,
					 public Thread 
{
public:
	SampleSource(ValueTree& value_tree, UndoManager* undo_manager);
	~SampleSource();
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

	void start();
	void stop();

	void set_file_path(const String path_to_swap) { file_path_ = path_to_swap; }
	void set_size(int num_channels, int num_samples);
	void set_position(uint32_t pos);
	void set_playing(bool playing) { is_playing_ = playing;  }
	bool is_playing() { return is_playing_;  }
	bool is_empty();

	// The directory where files are located. Hardcoded for now.
	CachedValue<String> resources_directory_; 
	CachedValue<String> file_path_;
private:
	void run() override;
	void check_for_path_to_open();

	SampleBuffer current_buffer_;

	bool is_playing_ = false;
	int position_ = 0;

	AudioFormatManager format_manager_;

	ValueTree state_;
};