#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_data_structures/juce_data_structures.h>

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//--------------------------------------------------------------------------------
/**
 * @class TreeView
 * @brief Display components corresponding to juce::ValueTrees in a tree view.
 *
 * This class does nothing besides creating item components and laying them out
 * in a tree structure. No scrolling, no drag and drop. Override createTreeViewItem
 * to create tree view item components
 *
 * @note it uses DirectoryModel keys for opening and closing directories, but it
 * does not require a DirectoryModel be used. This class can be used with any
 * juce::ValueTree.
 *
 * @see DirectoryModel
 * @see createTreeViewItem
 */
class TreeView
    : public juce::Component
    , private juce::ValueTree::Listener
{
public:
    void setValueTree(juce::ValueTree tree);

    void setRootItemVisible(bool showRootFolder);
    void setSubFolderIndentation(int pixels);
    void setItemMargin(int pixels);
    void setItemHeight(int pixels);

    void resized() override;

    int getTotalHeight() const;
    int getMaxIndentLevel() const;

private:
    void valueTreeChildAdded(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded) override;
    void valueTreeChildRemoved(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenRemoved,
                               int indexFromWhichChildWasRemoved) override;
    void valueTreeChildOrderChanged(juce::ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex,
                                    int newIndex) override;
    void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
                                  const juce::Identifier &property) override;

    struct Node
    {
        int getMaxIndentLevel() const;
        int getHeight(int itemHeight, int margin) const;
        void setBounds(juce::Rectangle<int> &bounds, int indent, int margin, int height);
        void initialize(juce::ValueTree t, const std::function<std::unique_ptr<juce::Component>(juce::ValueTree)> &fn);

        std::unique_ptr<juce::Component> component;
        juce::ValueTree tree;

        std::vector<Node> subNodes;
    };

    Node *getNode(juce::ValueTree tree);
    virtual std::unique_ptr<juce::Component> createTreeViewItem(juce::ValueTree tree) = 0;
    std::unique_ptr<juce::Component> internalCreateTreeViewItem(juce::ValueTree tree);

    Node m_root;

    bool m_showRootFolder{ false };
    int m_subFolderIndentation{ 15 };
    int m_itemMargin{ 2 };
    int m_itemHeight{ 20 };
};

//--------------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
