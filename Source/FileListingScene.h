//
// Created by bennet on 10/24/18.
//

#pragma once

#include "JuceHeader.h"
#include "FileList.h"
#include "Sequencer.h"

class FileListingScene : public Component //, ChangeListener
{
public:
    FileListingScene(const String& file_path, std::shared_ptr<Sequencer> sequencer_);

    void paint (Graphics& g) override;
    void resized() override;

private:
    std::unique_ptr<FileList> file_listing;
};

