#include "SampleEditorScene.h"



SampleEditorScene::SampleEditorScene(const ValueTree& value_tree, UndoManager* undo_manager)
	: ValueTreeObject(value_tree, undo_manager),
	  state_(value_tree),
	  thumbnail_cache_(5),
	  thumbnail_(512, format_manager_, thumbnail_cache_)
{
	// Eventually register the scene as a listener to thumbnail
	// using the ValueTree. That way a change in the sample means a change
	// in the thumbnail
	format_manager_.registerBasicFormats();
	thumbnail_.addChangeListener(this);
    sample_source_ = std::make_unique<SampleSource>(state_.getOrCreateChildWithName(IDs::SampleSource, undo_manager),
						undo_manager);

	addKeyListener(this);
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
	// Draw thumbnail clip
	g.setColour(Colours::darkslategrey);
	g.fillRect(thumbnail_bounds);
	g.setColour(Colours::lightgoldenrodyellow);                                    
	thumbnail_.drawChannels(g,                                     
		thumbnail_bounds,
		0.0,                          // start time
		thumbnail_.getTotalLength(),  // end time
		1.0f);                        // vertical zoom
	
	// Draw time scrubbers
	scrubber_min_position_ = 10;
	scrubber_max_position_ = getWidth() - 30;
	left_scrubber_.setBounds(scrubber_min_position_, getHeight() - 60, scrubber_width_, scrubber_height_);
	right_scrubber_.setBounds(scrubber_max_position_, getHeight() - 60, scrubber_width_, scrubber_height_);

	g.setColour(Colours::indianred);
	g.fillRect(left_scrubber_);
	g.fillRect(right_scrubber_);
}

void SampleEditorScene::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &thumbnail_)
	{
		repaint();
	}
}