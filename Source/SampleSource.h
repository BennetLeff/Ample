/*
  ==============================================================================

    SampleSource.h
    Created: 19 Sep 2018 4:58:00pm
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include <memory>
#include <optional>

#include "JuceHeader.h"

#include "RefCountedBufffer.h"

class SampleSource : public AudioTransportSource, public Thread 
{
public:
	SampleSource();
	~SampleSource();
	//void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	// void releaseResources() override;
	void run() override;
	void free_stale_buffers();
	void check_for_path_to_open();

	void set_size(int num_channels, int num_samples);
	void set_position(double pos) { position = pos;  }
	void set_playing(bool playing) { is_playing_ = playing;  }
	int get_position() { return position; }
	bool is_playing() { return is_playing_;  }
	bool is_empty();
	
	std::optional<std::shared_ptr<AudioSampleBuffer>> get_buffer();

	ReferenceCountedArray<RefCountedBuffer> buffers_;
	RefCountedBuffer::Ptr current_buffer_;
	String chosen_path_;
private:
	int position = 0;

	enum class PlaybackState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	} state_;

	bool is_playing_ = false;

	AudioFormatManager format_manager_;
};