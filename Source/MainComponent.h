#pragma once

#include <array>

#include "JuceHeader.h"

#include "SampleSource.h"
#include "Sequencer.h"
#include "SequencerTrack.h"

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
	TextButton play_button_;
	TextButton stop_button_;

	MixerAudioSource mixer_source_;
	SampleSource sampler_source_kick_;
	SampleSource sampler_source_snare_;
	Sequencer sequencer_;

	std::array<std::unique_ptr<SequencerButton>, NUM_SEQUENCER_STEPS> sample_assigners_;
	SequencerTrack grid_row_kick_{};
	SequencerTrack grid_row_snare_{};


	void change_state(PlayState new_state);
	void open_button_kick_clicked(); 
	void open_button_snare_clicked();
	void play_button_clicked();
	// void initialize_trigger_buttons_array(std::array<std::unique_ptr<SequencerButton>, NUM_SEQUENCER_STEPS>& button_array);
	void stop_button_clicked();
	// void trigger_button_color(uint16_t step_number);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
