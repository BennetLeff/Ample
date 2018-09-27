/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SampleSource.h"
#include "Sequencer.h"

#include <array>

#define NUM_SEQUENCER_STEPS 8

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent, public ChangeListener
{
public:
    MainComponent();
    ~MainComponent();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (Graphics& g) override;
    void resized() override;

	void changeListenerCallback(ChangeBroadcaster* source) override;
private:
	enum class PlayState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	} state_;

	TextButton open_button_kick_;
	TextButton open_button_snare_;

	std::array<std::unique_ptr<TextButton>, 8> sample_assigners_;

	TextButton play_button_;
	TextButton stop_button_;

	SampleSource sampler_source_kick_;
	SampleSource sampler_source_snare_;
	MixerAudioSource mixer_source_;
	Sequencer sequencer_;

	void change_state(PlayState new_state);
	void open_button_kick_clicked(); 
	void open_button_snare_clicked();
	void play_button_clicked();
	void stop_button_clicked();
	void trigger_button_color(uint16_t step_number);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
