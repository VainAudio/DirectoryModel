#pragma once

//--------------------------------------------------------------------------------

#include <juce_gui_basics/juce_gui_basics.h>
#include <vdm_directory/vdm_directory.h>
#include <vdm_ui/vdm_ui.h>

//--------------------------------------------------------------------------------

class DirectoryModelDemoJuceTreeViewItemComponent
    : public juce::Component
    , private juce::ValueTree::Listener
    , private vdm::TreeViewItemBase
{
public:
    explicit DirectoryModelDemoJuceTreeViewItemComponent(juce::ValueTree tree);
    ~DirectoryModelDemoJuceTreeViewItemComponent() override;

    void paint(juce::Graphics &g) override;

    void mouseUp(const juce::MouseEvent &event) override;
    void mouseDoubleClick(const juce::MouseEvent &event) override;

    void onItemSelectedChanged(bool isSelected) override;
    void onItemIsDirOpenChanged(bool isDirOpen) override;

private:
    void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
                                  const juce::Identifier &property) override;

    juce::ValueTree m_tree;
    juce::String m_sizeString;
    bool m_isSelected{ false };
};

//--------------------------------------------------------------------------------
