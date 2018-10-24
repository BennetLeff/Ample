//
// Created by bennet on 10/24/18.
//

#pragma once

#include "JuceHeader.h"
#include "FileList.h"

class FileListingScene : public Component
{
public:
    FileListingScene(const String& file_path);

    void paint (Graphics& g) override;
    void resized() override;

private:
    std::unique_ptr<FileList> file_listing;
};

