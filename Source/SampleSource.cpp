/*
  ==============================================================================

    SampleSource.cpp
    Created: 19 Sep 2018 4:58:00pm
    Author:  bennet

  ==============================================================================
*/

#include "SampleSource.h"

SampleSource::SampleSource()
{
	/* Initiate an empty AudioSampleBuffer whose size should be set in the MainComponent open file method. */
	audio_buffer_ = std::make_shared<AudioSampleBuffer>(0, 0);
}

void SampleSource::getNextAudioBlock(const AudioSourceChannelInfo& buffer_to_fill)
{
	if (!is_playing())
	{
		buffer_to_fill.clearActiveBufferRegion();
		return;
	}

	int num_in_channels = audio_buffer_->getNumChannels();
	int num_out_channels = buffer_to_fill.buffer->getNumChannels();

	int out_samples_remaining = buffer_to_fill.numSamples;
	int out_samples_offset = buffer_to_fill.startSample;

	while (out_samples_remaining > 0)
	{
		int buffer_samples_remaining = audio_buffer_->getNumSamples() - get_position();
		int samples_this_iter = jmin(out_samples_remaining, buffer_samples_remaining);

		for (int channel = 0; channel < num_out_channels; ++channel)
		{
			buffer_to_fill.buffer->copyFrom(channel,
				out_samples_offset,
				*(audio_buffer_.get()),
				channel % num_in_channels,
				get_position(),
				samples_this_iter);
		}
	
		out_samples_remaining -= samples_this_iter;
		out_samples_offset += samples_this_iter;
		set_position(get_position() + samples_this_iter);

		if (get_position() == audio_buffer_->getNumSamples())
			set_position(0);
	}
}