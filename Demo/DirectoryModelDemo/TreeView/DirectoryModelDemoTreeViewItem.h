#pragma once

//--------------------------------------------------------------------------------

#include <juce_gui_basics/juce_gui_basics.h>
#include <vdm_directory/vdm_directory.h>
#include <vdm_ui/vdm_ui.h>

#include "../SelectionViewer.h"

//--------------------------------------------------------------------------------

class DirectoryModelDemoTreeViewItem
    : public juce::Component
    , private vdm::TreeViewItemBase
    , private juce::ValueTree::Listener
{
public:
    explicit DirectoryModelDemoTreeViewItem(juce::ValueTree tree);
    ~DirectoryModelDemoTreeViewItem() override;

    void paint(juce::Graphics &g) override;

    void mouseUp(const juce::MouseEvent &event) override;
    void mouseEnter(const juce::MouseEvent &event) override;
    void mouseExit(const juce::MouseEvent &event) override;
    void mouseDoubleClick(const juce::MouseEvent &event) override;

private:
    // vdm::TreeViewItem
    void onItemSelectedChanged(bool isSelected) override;
    void onItemIsDirOpenChanged(bool isDirOpen) override;

    // juce::ValueTree::Listener
    void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
                                  const juce::Identifier &property) override;

    juce::ValueTree m_tree;
    bool m_isSelected{ false };
    juce::String m_sizeString;
};

//--------------------------------------------------------------------------------
