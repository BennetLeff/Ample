#include "MainComponent.h"
#include "Sequencer.h"

#include <vector>
#include <algorithm>

MainComponent::MainComponent()
	: state_(PlayState::Stopped)
{
    // specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);

//    main_scene = std::make_unique<MainScene>();
//    addAndMakeVisible(main_scene.get());
//    main_scene->setSize(MAIN_COMP_WIDTH, MAIN_COMP_HEIGHT);

    file_listing_scene = std::make_unique<FileListingScene>(xml_file_path_);
    addAndMakeVisible(file_listing_scene.get());
    file_listing_scene->setSize(MAIN_COMP_WIDTH, MAIN_COMP_HEIGHT);


    addKeyListener(this);

	// Make sure you set the size of the component after
	// you add any child components.
	setSize(MAIN_COMP_WIDTH, MAIN_COMP_HEIGHT);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	// mixer_source_.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // main_scene->play(bufferToFill);
}

void MainComponent::releaseResources()
{
}

void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{

}

bool MainComponent::keyPressed(const KeyPress& key, Component* originating_component)
{
    // Logger::writeToLog("Key Pressed... ");

    if (key == key.leftKey)
    {
        // main_component->setVisible(false);
        Logger::writeToLog("Left Key Pressed");

    }

    else if (key == key.rightKey)
    {
        Logger::writeToLog("Right Key Pressed");
    }

    return true;
}

bool MainComponent::keyStateChanged(bool is_key_down, Component* originating_component)
{

    return true;
}
