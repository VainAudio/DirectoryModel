#pragma once

//--------------------------------------------------------------------------------

#include <juce_gui_basics/juce_gui_basics.h>
#include <vdm_directory/vdm_directory.h>
#include <vdm_ui/vdm_ui.h>

//--------------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//--------------------------------------------------------------------------------

class JuceTreeViewItemComponent
    : public juce::Component
    , private juce::ValueTree::Listener
    , private vdm::TreeViewItemBase
{
public:
    explicit JuceTreeViewItemComponent(juce::ValueTree tree);
    ~JuceTreeViewItemComponent() override;

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

END_VDM_NAMESPACE

//--------------------------------------------------------------------------------
