#pragma once

//--------------------------------------------------------------------------------

#include <juce_gui_basics/juce_gui_basics.h>
#include <vdm_directory/vdm_directory.h>
#include <vdm_ui/vdm_ui.h>

//--------------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//--------------------------------------------------------------------------------

class JuceTreeViewItem : public vdm::JuceTreeView::Item
{
public:
    explicit JuceTreeViewItem(juce::ValueTree tree);
    ~JuceTreeViewItem() override;

private:
    std::unique_ptr<juce::TreeViewItem> createItem(juce::ValueTree) override;
    std::unique_ptr<juce::Component> createItemComponent(juce::ValueTree) override;
};

//--------------------------------------------------------------------------------

END_VDM_NAMESPACE

//--------------------------------------------------------------------------------
