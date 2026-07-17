#pragma once

//--------------------------------------------------------------------------------

#include <juce_gui_basics/juce_gui_basics.h>
#include <vdm_directory/vdm_directory.h>
#include <vdm_ui/vdm_ui.h>

//--------------------------------------------------------------------------------

class DirectoryModelDemoJuceTreeViewItem : public vdm::JuceTreeView::Item
{
public:
    explicit DirectoryModelDemoJuceTreeViewItem(juce::ValueTree tree);
    ~DirectoryModelDemoJuceTreeViewItem() override;

private:
    std::unique_ptr<juce::TreeViewItem> createItem(juce::ValueTree) override;
    std::unique_ptr<juce::Component> createItemComponent(juce::ValueTree) override;
};

//--------------------------------------------------------------------------------
