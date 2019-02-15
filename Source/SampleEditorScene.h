#pragma once

#include "JuceHeader.h"

#include "SampleSource.h"

class SampleEditorScene : public Component,
	public ValueTreeObject<IDs::SampleEditorScene>,
	public ChangeListener
{
public:
	SampleEditorScene(const ValueTree& value_tree, UndoManager* undo_manager);
	~SampleEditorScene();

	void paint(Graphics& g) override;
	void resized() override;
	void set_sample(const String& sample_file_path);

private:
	void changeListenerCallback(ChangeBroadcaster* source) override;
	void paint_if_no_file_loaded(Graphics& g, const Rectangle<int>& thumbnail_bounds);
	void paint_if_file_loaded(Graphics& g, const Rectangle<int>& thumbnail_bounds);

	AudioFormatManager format_manager_;
	AudioThumbnailCache thumbnail_cache_;
	AudioThumbnail thumbnail_;

	String sample_file_path_;
	File sample_file_;
	// internal sample source used for playback within sample editing
	std::unique_ptr<SampleSource> sample_source_;

	static const juce::uint16 scrubber_width_ = 20;
	static const juce::uint16 scrubber_height_ = 40;
	juce::uint16 scrubber_min_position_;
	juce::uint16 scrubber_max_position_;
	
	Rectangle<int> left_scrubber_;
	Rectangle<int> right_scrubber_;

	ValueTree state_;
};

