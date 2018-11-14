//
// Created by bennet on 10/20/18.
//

#include "MainScene.h"

MainScene::MainScene(std::shared_ptr<Sequencer> sequencer)
	 : sequencer_(sequencer), volume_slider_(Slider::Rotary, Slider::TextBoxBelow),
	    tempo_slider_(Slider::Rotary, Slider::TextBoxBelow)
{
    addAndMakeVisible(volume_slider_);
    addAndMakeVisible(tempo_slider_);

    volume_slider_.setRange(Range<double>(-60.0, 6.0), 1.0);
    tempo_slider_.setRange(Range<double>(20.0, 400.0), 1.0);
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
    // volume_slider_.setBounds(200, 200, 200, 200);
    // tempo_slider_.setBounds(0, 0, 200, 200);

    FlexBox fb;                                          // [1]
    fb.flexWrap = FlexBox::Wrap::wrap;                   // [2]
    fb.justifyContent = FlexBox::JustifyContent::center; // [3]
    fb.alignContent = FlexBox::AlignContent::center;     // [4]
    fb.items.add(FlexItem(tempo_slider_).withMinWidth(140.0f).withMinHeight(140.0f));
    fb.items.add(FlexItem(volume_slider_).withMinWidth(140.0f).withMinHeight(140.0f));
    fb.performLayout (getLocalBounds().toFloat());       // [6]
}