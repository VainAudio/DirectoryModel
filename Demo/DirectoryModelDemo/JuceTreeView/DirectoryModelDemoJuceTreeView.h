#pragma once

//--------------------------------------------------------------------------------

#include <juce_gui_basics/juce_gui_basics.h>
#include <vdm_directory/vdm_directory.h>
#include <vdm_ui/vdm_ui.h>

//--------------------------------------------------------------------------------

class DirectoryModelDemoJuceTreeView : public vdm::JuceTreeView
{
private:
    std::unique_ptr<vdm::JuceTreeView::Item> createTreeViewItem(juce::ValueTree tree) override;
};

//--------------------------------------------------------------------------------
