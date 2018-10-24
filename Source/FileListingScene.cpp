//
// Created by bennet on 10/24/18.
//

#include "FileListingScene.h"

FileListingScene::FileListingScene(const String &file_path)
{
    file_listing = std::make_unique<FileList>(file_path);
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
