//
// Created by bennet on 10/20/18.
//

#include "MainScene.h"

MainScene::MainScene(std::shared_ptr<Sequencer> sequencer)
	 : sequencer_(sequencer)
{
	setup_text_button(play_button_, [this] { Logger::writeToLog("Play button clicked"); }, "Play [Does nothing atm]", Colours::green, true);
	setup_text_button(stop_button_, [this] { Logger::writeToLog("Stop button clicked");  }, "Stop [Does nothing atm]", Colours::red, true);

	addAndMakeVisible(&play_button_);
	addAndMakeVisible(&stop_button_);
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
}

void MainScene::setup_text_button(TextButton& button, std::function<void()> on_click, const String& text, const Colour& colour, const bool on_or_off)
{
	button.onClick = on_click;
	button.setButtonText(text);
	button.setColour(TextButton::buttonColourId, colour);
	button.setEnabled(on_or_off);
}
