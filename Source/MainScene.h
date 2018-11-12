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
    MainScene(std::shared_ptr<Sequencer> sequencer);
    void play(const AudioSourceChannelInfo& bufferToFill);

    void paint (Graphics& g) override;
    void resized() override;

private:
    void play_button_clicked();
    void setup_text_button(TextButton& button, std::function<void()> on_click, const String& text, const Colour& colour, const bool on_or_off);
    void stop_button_clicked();

    TextButton play_button_;
    TextButton stop_button_;

    MixerAudioSource mixer_source_;

    std::shared_ptr<Sequencer> sequencer_;
};