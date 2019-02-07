#include "SampleEditorScene.h"



SampleEditorScene::SampleEditorScene()
	: thumbnail_cache_(5),
	  thumbnail_(512, format_manager_, thumbnail_cache_)
{
	// Eventually register the scene as a listener to thumbnail
	// using the ValueTree. That way a change in the sample means a change
	// in the thumbnail
	format_manager_.registerBasicFormats();
	thumbnail_.addChangeListener(this);
}

SampleEditorScene::~SampleEditorScene()
{
}

void SampleEditorScene::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);
	if (thumbnail_.getNumChannels() == 0)
		paint_if_no_file_loaded(g, thumbnailBounds);
	else
		paint_if_file_loaded(g, thumbnailBounds);
}

void SampleEditorScene::resized()
{
	
}

void SampleEditorScene::set_sample(const String& sample_file_path)
{
	sample_file_path_ = sample_file_path;
	sample_file_ = File(sample_file_path);
	thumbnail_.setSource(new FileInputSource(sample_file_));
}

void SampleEditorScene::paint_if_no_file_loaded(Graphics& g, const Rectangle<int>& thumbnail_bounds)
{
	g.setColour(Colours::darkgrey);
	g.fillRect(thumbnail_bounds);
	g.setColour(Colours::white);
	g.drawFittedText("No File Loaded", thumbnail_bounds, Justification::centred, 1.0f);
}

void SampleEditorScene::paint_if_file_loaded(Graphics& g, const Rectangle<int>& thumbnail_bounds)
{
	g.setColour(Colours::white);
	g.fillRect(thumbnail_bounds);
	g.setColour(Colours::red);                                     // [8]
	thumbnail_.drawChannels(g,                                      // [9]
		thumbnail_bounds,
		0.0,                                    // start time
		thumbnail_.getTotalLength(),             // end time
		1.0f);                                  // vertical zoom
}

void SampleEditorScene::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &thumbnail_)
	{
		repaint();
	}
}