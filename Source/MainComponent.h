/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SampleSource.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, public ChangeListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

private:
    //==============================================================================

	enum class TransportState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	} state_;

	TextButton open_button_;
	TextButton play_button_;
	TextButton stop_button_;

	SampleSource sampler_source_;

	int32_t position;

	void change_state(TransportState new_state);
	void open_button_clicked();
	void play_button_clicked();
	void stop_button_clicked();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
