#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_data_structures/juce_data_structures.h>

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//--------------------------------------------------------------------------------
/**
 * @class JuceTreeView
 * @brief Integrates a DirectoryModel value tree into a juce::TreeView.
 *
 * This whole class is kind of annoying. It's only really useful if your code is
 * tightly coupled to a juce::TreeView and you need it to work with a
 * DirectoryModel / SelectionModel. Otherwise, prefer TreeView.
 *
 * @see JuceTreeViewItem
 */
class JuceTreeView : public juce::TreeView
{
public:
    ~JuceTreeView() override;

    void setValueTree(juce::ValueTree tree);

    /**
     * @class JuceTreeView::Item
     * @brief Adds this library's open/closed/selected logic to a juce::TreeView.
     *
     * @see JuceTreeView
     */
    class Item
        : public juce::TreeViewItem
        , private juce::ValueTree::Listener
    {
    public:
        Item();
        ~Item() override;

        void setValueTree(juce::ValueTree tree);

        bool mightContainSubItems() override;

        std::unique_ptr<juce::Component> createItemComponent() final;

        void itemOpennessChanged(bool isNowOpen) override;
        void itemSelectionChanged(bool isNowSelected) override;

    private:
        void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
                                      const juce::Identifier &property) override;
        void valueTreeChildAdded(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded) override;
        void valueTreeChildRemoved(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenRemoved,
                                   int indexFromWhichChildWasRemoved) override;
        void valueTreeChildOrderChanged(juce::ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex,
                                        int newIndex) override;

        virtual std::unique_ptr<juce::TreeViewItem> createItem(juce::ValueTree);
        virtual std::unique_ptr<juce::Component> createItemComponent(juce::ValueTree);

        juce::ValueTree m_tree;
    };

private:
    virtual std::unique_ptr<Item> createTreeViewItem(juce::ValueTree tree);
    std::unique_ptr<Item> m_p;
};

//--------------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
