#pragma once

#include "JuceHeader.h"

#include "SampleSource.h"

class SampleEditorScene : public Component,
						  public ChangeListener
{
public:
	SampleEditorScene();
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
};

