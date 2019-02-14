/*
  ==============================================================================

    SampleSource.cpp
    Created: 19 Sep 2018 4:58:00pm
    Author:  bennet

  ==============================================================================
*/
#include "SampleSource.h"

SampleSource::SampleSource(const ValueTree& value_tree, UndoManager* undo_manager)
	: ValueTreeObject(value_tree, undo_manager),
	is_playing_(false),
	position_(0),
	file_path_(""),
	resources_directory_(get_state(), IDs::SampleSourceProps::resources_directory, get_undo_manager(),
	"C:\\Users\\bennet\\Documents\\Workspace\\Ample\\Resources\\"),
	state_(value_tree),
	Thread("")
{
	jassert(value_tree.hasType(IDs::SampleSource));
	resources_directory_.referTo(state_, IDs::SampleSourceProps::resources_directory, get_undo_manager());

	state_.setProperty(IDs::SampleSourceProps::file_path, file_path_, get_undo_manager());

	format_manager_.registerBasicFormats();
	startThread();
}

SampleSource::~SampleSource() 
{
	releaseResources();
	stopThread(500);
}

void SampleSource::getNextAudioBlock(const AudioSourceChannelInfo& buffer_to_fill)
{
	if (!is_playing())
	{
		buffer_to_fill.clearActiveBufferRegion();
		return;
	}

	// Once we know we're actually playing audio we can proceed with loading and playing data.
	if (current_buffer_.buffer_ == nullptr)
	{
		buffer_to_fill.clearActiveBufferRegion();
		return;
	}

	int32_t position = current_buffer_.position_;

	int32_t num_in_channels = current_buffer_.buffer_->getNumChannels();
	int32_t num_out_channels = buffer_to_fill.buffer->getNumChannels();

	int32_t out_samples_remaining = buffer_to_fill.numSamples;
	int32_t out_samples_offset = 0;

	while (out_samples_remaining > 0)
	{
		int32_t buffer_samples_remaining = current_buffer_.buffer_->getNumSamples() - position_;
		int32_t samples_this_iter = jmin(out_samples_remaining, buffer_samples_remaining);

		for (int32_t channel = 0; channel < num_out_channels; ++channel)
		{
			buffer_to_fill.buffer->copyFrom(channel,
				buffer_to_fill.startSample + out_samples_offset,
				*(current_buffer_.buffer_.get()),
				channel % num_in_channels,
				position_,
				samples_this_iter);
		}
	
		/*
		 *	These operations are a bit like moving an audio "scrubber". 
		 *  Each step we move our needle, the position, along a bit further.
		 *  This lets us keep track of where we are in the audio so that JUCE 
		 *  can call getNextAudioBlock over and over while processing audio, and 
		 *  "scrubbing" along.
		 */
		out_samples_remaining -= samples_this_iter;
		out_samples_offset += samples_this_iter;
		set_position(position_ + samples_this_iter); 

		/*
		 * If we get to the end of the sample buffer, we can stop playing.
		 */
		if (position_ == current_buffer_.buffer_->getNumSamples())
		{
			// Logger::writeToLog("stopped playing sample .... ");
			stop();
			buffer_to_fill.clearActiveBufferRegion();
			return;
		}
	}

	current_buffer_.position_ = position;
}

void SampleSource::run()
{
	while (!threadShouldExit())
	{
		check_for_path_to_open();
		wait(500);
	}
}

void SampleSource::start()
{
	if (!is_playing_ && !is_empty())
	{
		/* 
		 * Once is_playing_ is set to true, the getNextAudioBlock method should
		 * be able to start playing audio.
		 */
		is_playing_ = true;
		// sendChangeMessage();
	}
}

void SampleSource::stop()
{
	if (is_playing_)
	{
		is_playing_ = false;
		position_ = 0;
	} 
}

void SampleSource::check_for_path_to_open()
{
	String path_to_open;
	path_to_open = file_path_;
	if (path_to_open.isNotEmpty())
	{
		File file(path_to_open);
		std::unique_ptr<AudioFormatReader> reader(format_manager_.createReaderFor(file));
		if (reader.get() != nullptr)
		{
			auto duration = reader->lengthInSamples / reader->sampleRate;
			if (duration < 2)
			{
				auto temp_buffer = std::make_unique<AudioSampleBuffer>(reader->numChannels, reader->lengthInSamples);
				reader->read(temp_buffer.get(), 0, reader->lengthInSamples, 0, true, true);
				current_buffer_.buffer_ = std::move(temp_buffer);
				current_buffer_.position_ = 0;
			}
			else
			{
				// handle the error that the file is 2 seconds or longer..
			}
		}
	}
}

bool SampleSource::is_empty()
{
	if (current_buffer_.buffer_ == nullptr)
		return true;
	else
		return (current_buffer_.buffer_->getNumChannels() == 0) && (current_buffer_.buffer_->getNumSamples() == 0);
}

void SampleSource::set_size(int num_channels, int num_samples)
{
	if (current_buffer_.buffer_ != nullptr)
		current_buffer_.buffer_->setSize(num_channels, num_samples);
}

void SampleSource::set_position(uint32_t pos)
{
	position_ = pos;
}

void SampleSource::set_file_path(const String& path_to_swap)
{
	file_path_ = path_to_swap;
	state_.setProperty(IDs::SampleSourceProps::file_path, file_path_, get_undo_manager());
}

void SampleSource::prepareToPlay(int samples_per_block_expected, double sample_rate)
{
	/* Do nothing? */
}

void SampleSource::releaseResources()
{
	/* Do nothing? */
}