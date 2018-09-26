/*
  ==============================================================================

    RefCountedBufffer.h
    Created: 22 Sep 2018 7:11:34pm
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include <functional>
#include <optional>

#include "JuceHeader.h"

class RefCountedBuffer : public ReferenceCountedObject {
 public:
  typedef ReferenceCountedObjectPtr<RefCountedBuffer> Ptr;
  RefCountedBuffer(const String& name_to_use, int num_channels, int num_samples)
      : name_(name_to_use) {
    buffer_ = std::make_shared<AudioSampleBuffer>(num_channels, num_samples);
  }

  const std::shared_ptr<AudioSampleBuffer>& get_audio_sample_buffer() {
    return buffer_;
  }

  double position_ = 0;

 private:
  String name_;
  std::shared_ptr<AudioSampleBuffer> buffer_;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RefCountedBuffer)
};
