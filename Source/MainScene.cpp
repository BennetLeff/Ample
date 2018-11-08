//
// Created by bennet on 10/20/18.
//

#include "MainScene.h"

MainScene::MainScene(std::shared_ptr<Sequencer> sequencer)
	 : sequencer_(sequencer)
{
	setup_text_button(play_button_, [this] { /* does nothing */ }, "Play [Does nothing atm]", Colours::green, false);
	setup_text_button(stop_button_, [this] { /* does nothing */ }, "Stop [Does nothing atm]", Colours::red, false);

	addAndMakeVisible(&play_button_);
	addAndMakeVisible(&stop_button_);
	addAndMakeVisible(sequencer_.get());
}

void MainScene::play(const AudioSourceChannelInfo& bufferToFill)
{
    mixer_source_.getNextAudioBlock(bufferToFill);
}

void MainScene::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainScene::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	play_button_.setBounds(10, 70, getWidth() - 20, 20);
	stop_button_.setBounds(10, 100, getWidth() - 20, 20);
    sequencer_->setBounds(getLocalBounds());
}

void MainScene::setup_text_button(TextButton& button, std::function<void()> on_click, const String& text, const Colour& colour, const bool on_or_off)
{
	button.onClick = on_click;
	button.setButtonText(text);
	button.setColour(TextButton::buttonColourId, colour);
	button.setEnabled(on_or_off);
}

void MainScene::set_sample_sources(std::vector<SampleSource> &sample_sources)
{
//    for (auto i = 0; i < sequencer_tracks_.size(); i++)
//    {
//        auto& sample_source = sample_sources.at(i);
//        sequencer_tracks_.at(i)->attach_sample(sample_source);
//        mixer_source_.addInputSource(&sample_source, false);
//    }
}