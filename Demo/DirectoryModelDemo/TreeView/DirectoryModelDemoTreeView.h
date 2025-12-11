#pragma once

//--------------------------------------------------------------------------------

#include <juce_gui_basics/juce_gui_basics.h>
#include <vdm_directory/vdm_directory.h>
#include <vdm_ui/vdm_ui.h>

//--------------------------------------------------------------------------------

class DirectoryModelDemoTreeView : public vdm::TreeView
{
public:
    DirectoryModelDemoTreeView();
    ~DirectoryModelDemoTreeView() override;

    void setRootItemSelectable(bool b);
    void setSelectionValueTree(juce::ValueTree tree);

private:
    std::unique_ptr<juce::Component> createTreeViewItem(juce::ValueTree tree) override;
    vdm::TreeViewSelectorKeyListener m_dirSelection;
};

//--------------------------------------------------------------------------------
