//
// Created by bennet on 10/24/18.
//

#pragma once

#include "JuceHeader.h"
#include "FileList.h"

class FileListingScene : public Component
{
public:
    FileListingScene(const String& file_path, ChangeListener* file_path_change_listener);

    void paint (Graphics& g) override;
    void resized() override;

private:
    std::unique_ptr<FileList> file_listing;
};

