//
// Created by bennet on 10/20/18.
//

#include "MainScene.h"

MainScene::MainScene(std::shared_ptr<Sequencer> sequencer)
	 : sequencer_(sequencer), volume_slider_(Slider::Rotary, Slider::TextBoxBelow),
	    tempo_slider_(Slider::Rotary, Slider::TextBoxBelow)
{
	tempo_label_.setText("Tempo", dontSendNotification);
	volume_label_.setText("Volume", dontSendNotification);

	tempo_slider_.onValueChange = [this] { sequencer_->update_tempo(tempo_slider_.getValue());  };
	volume_slider_.onValueChange = [] {}; 

	addAndMakeVisible(volume_label_);
	addAndMakeVisible(volume_slider_);
    addAndMakeVisible(tempo_label_);
    addAndMakeVisible(tempo_slider_);

    volume_slider_.setRange(Range<double>(-60.0, 6.0), 1.0);
    tempo_slider_.setRange(Range<double>(20.0, 400.0), 1.0);
	tempo_slider_.setValue(sequencer_->get_tempo());
    tempo_slider_.setSkewFactor(0.5);
}

void MainScene::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainScene::resized()
{
	Grid grid;
	using Track = Grid::TrackInfo;
	grid.templateRows = { Track(50_px), Track(50_px) }; // Add one "track" per row
	grid.templateColumns = { Track(140_px), Track(140_px) }; // Add one "track" per column

	Array<GridItem> main_scene_grid_items_;
	// Set up the first row
	main_scene_grid_items_.add( GridItem(volume_label_).withWidth(50.0f).withHeight(50.0f) );
	main_scene_grid_items_.add( GridItem(tempo_label_).withWidth(50.0f).withHeight(50.0f) );
	// Set up the second row
	main_scene_grid_items_.add(GridItem(volume_slider_).withWidth(140.0f).withHeight(140.0f));
	main_scene_grid_items_.add( GridItem(tempo_slider_).withWidth(140.0f).withHeight(140.0f) );

	grid.items = main_scene_grid_items_;

	grid.performLayout(getLocalBounds());
}