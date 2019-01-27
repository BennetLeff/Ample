//
// Created by bennet on 10/20/18.
//

#pragma once

#include "JuceHeader.h"

#include "SampleSource.h"
#include "Sequencer.h"
#include "SequencerTrack.h"

class MainScene : public Component, public ChangeBroadcaster
{
public:
    MainScene(std::shared_ptr<Sequencer> sequencer);

    void paint (Graphics& g) override;
    void resized() override;

private:
	Label tempo_label_;
	Label volume_label_;
    Slider volume_slider_;
    Slider tempo_slider_;

	MixerAudioSource mixer_source_;

	SampleSource sample_ = SampleSource();

    std::shared_ptr<Sequencer> sequencer_;
};