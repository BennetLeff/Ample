//
// Created by bennet on 10/24/18.
//

#include "FileListingScene.h"

FileListingScene::FileListingScene(const String &file_path, std::shared_ptr<Sequencer> sequencer)
{
    file_listing = std::make_unique<FileList>(file_path, sequencer);
    addAndMakeVisible(file_listing.get());
}

void FileListingScene::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void FileListingScene::resized()
{
    file_listing->setBounds(getBounds());
}
