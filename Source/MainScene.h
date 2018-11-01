//
// Created by bennet on 10/20/18.
//

#pragma once

#include "JuceHeader.h"

#include "SampleSource.h"
#include "Sequencer.h"
#include "SequencerTrack.h"

class MainScene : public Component
{
public:
    MainScene();
    void play(const AudioSourceChannelInfo& bufferToFill);

    void paint (Graphics& g) override;
    void resized() override;

    static const uint16_t num_sequencer_tracks_ = 2;
    std::array< std::unique_ptr<SequencerTrack>, num_sequencer_tracks_> sequencer_tracks_;

private:
    void open_button_kick_clicked();
    void open_button_snare_clicked();
    void play_button_clicked();
    void setup_text_button(TextButton& button, std::function<void()> on_click, const String& text, const Colour& colour, const bool on_or_off);
    void stop_button_clicked();

    // TextButton open_button_kick_;
    // TextButton open_button_snare_;
    TextButton play_button_;
    TextButton stop_button_;

    MixerAudioSource mixer_source_;
    SampleSource sampler_source_kick_;
    SampleSource sampler_source_snare_;
    std::shared_ptr<Sequencer> sequencer_;
};