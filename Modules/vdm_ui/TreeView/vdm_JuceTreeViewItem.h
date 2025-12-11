#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_data_structures/juce_data_structures.h>

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//--------------------------------------------------------------------------------
/**
 * @class JuceTreeViewItem
 * @brief Adds this library's open/closed/selected logic to a juce::TreeView.
 *
 * @see JuceTreeView
 */
class JuceTreeViewItem
    : public juce::TreeViewItem
    , private juce::ValueTree::Listener
{
public:
    JuceTreeViewItem();
    ~JuceTreeViewItem() override;

    void setValueTree(juce::ValueTree tree);

    bool mightContainSubItems() override;

    std::unique_ptr<juce::Component> createItemComponent() final;

    void itemOpennessChanged(bool isNowOpen) override;
    void itemSelectionChanged(bool isNowSelected) override;

private:
    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
    void valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override;
    void valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
    void valueTreeChildOrderChanged(juce::ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override;

    virtual std::unique_ptr<juce::TreeViewItem> createItem(juce::ValueTree) = 0;
    virtual std::unique_ptr<juce::Component> createItemComponent(juce::ValueTree) = 0;

    juce::ValueTree m_tree;
};

//--------------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
