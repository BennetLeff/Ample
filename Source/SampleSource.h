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


class SampleSource : public AudioTransportSource
{
public:
	SampleSource();
	//void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	// void releaseResources() override;

	void set_size(int num_channels, int num_samples) { audio_buffer_->setSize(num_channels, num_samples); }
	void set_position(double pos) { position = pos;  }
	void set_playing(bool playing) { is_playing_ = playing;  }
	int get_position() { return position; }
	bool is_playing() { return is_playing_;  }
	bool is_empty() { return (audio_buffer_->getNumChannels() == 0) && (audio_buffer_->getNumSamples() == 0); }
	
	std::shared_ptr<AudioSampleBuffer> get_buffer() { return audio_buffer_;  }
private:
	std::shared_ptr<AudioSampleBuffer> audio_buffer_;

	int position = 0;

	enum class PlaybackState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	} state_;

	bool is_playing_ = false;
};