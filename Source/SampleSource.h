/*
  ==============================================================================

    SampleSource.h
    Created: 19 Sep 2018 4:58:00pm
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class SampleSource : public AudioTransportSource
{
public:
	// void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	// void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	// void releaseResources() override;

	void set_size(int num_channels, int num_samples) { audio_buffer_.setSize(num_channels, num_samples); }
	void set_position(double pos) { setPosition(pos);  }
	bool is_playing() { return isPlaying();  }

private:
	AudioSampleBuffer audio_buffer_;

	enum class PlaybackState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	} state_;
};