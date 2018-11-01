//
// Created by bennet on 10/24/18.
//

#include "FileListingScene.h"

FileListingScene::FileListingScene(const String &file_path, ChangeListener* file_path_change_listener)
{
    file_listing = std::make_unique<FileList>(file_path, file_path_change_listener);
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
