#include "vdm_JuceTreeView.h"

#include "vdm_JuceTreeViewItem.h"
#include "vdm_JuceTreeViewItemComponent.h"

#include <vdm_directory/vdm_directory.h>

//--------------------------------------------------------------------------------

vdm::JuceTreeView::~JuceTreeView()
{
    setRootItem(nullptr);
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeView::setValueTree(juce::ValueTree tree)
{
    m_p = createTreeViewItem(tree);
    setRootItem(m_p.get());
}

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

vdm::JuceTreeView::Item::Item() = default;

//--------------------------------------------------------------------------------

vdm::JuceTreeView::Item::~Item()
{
    m_tree.removeListener(this);
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeView::Item::setValueTree(juce::ValueTree tree)
{
    clearSubItems();

    m_tree = tree;
    m_tree.addListener(this);
    for (int i = 0; i < tree.getNumChildren(); i++)
    {
        auto child = tree.getChild(i);
        vdm::JuceTreeView::Item::valueTreeChildAdded(tree, child);
    }
    if (vdm::DirectoryModel::IsDirOpen(tree))
        setOpen(true);
    if (vdm::SelectionModel::IsSelected(tree))
        setSelected(true, false, juce::NotificationType::dontSendNotification);
}

//--------------------------------------------------------------------------------

bool vdm::JuceTreeView::Item::mightContainSubItems()
{
    return vdm::DirectoryModel::IsDir(m_tree);
    return m_tree.getNumChildren() > 0;
}

//--------------------------------------------------------------------------------

std::unique_ptr<juce::Component> vdm::JuceTreeView::Item::createItemComponent()
{
    return createItemComponent(m_tree);
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeView::Item::itemOpennessChanged(bool isNowOpen)
{
    if (vdm::DirectoryModel::IsDir(m_tree))
        vdm::DirectoryModel::SetDirOpen(m_tree, isNowOpen);
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeView::Item::itemSelectionChanged(bool isNowSelected)
{
    vdm::SelectionModel::RequestSelectTree(m_tree, isNowSelected);
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeView::Item::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
                                                       const juce::Identifier &property)
{
    if (treeWhosePropertyHasChanged == m_tree)
    {
        if (property == vdm::DirectoryModel::Keys::DirOpen)
            setOpen(vdm::DirectoryModel::IsDirOpen(m_tree));
        if (property == vdm::SelectionModel::IsSelectedKey)
            setSelected(vdm::SelectionModel::IsSelected(m_tree), false, juce::NotificationType::dontSendNotification);
    }
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeView::Item::valueTreeChildAdded(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded)
{
    if (parentTree == m_tree)
    {
        auto ptr = createItem(childWhichHasBeenAdded);
        addSubItem(ptr.release());
    }
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeView::Item::valueTreeChildRemoved(juce::ValueTree &parentTree,
                                                    juce::ValueTree &childWhichHasBeenRemoved,
                                                    int indexFromWhichChildWasRemoved)
{
    juce::ignoreUnused(childWhichHasBeenRemoved);

    if (parentTree == m_tree)
    {
        removeSubItem(indexFromWhichChildWasRemoved);
    }
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeView::Item::valueTreeChildOrderChanged(juce::ValueTree &parentTree, int oldIndex, int newIndex)
{
    if (parentTree == m_tree)
    {
        const auto item{ getSubItem(oldIndex) };
        removeSubItem(oldIndex, false);
        addSubItem(item, newIndex);
    }
}

//--------------------------------------------------------------------------------

std::unique_ptr<juce::TreeViewItem> vdm::JuceTreeView::Item::createItem(juce::ValueTree tree)
{
    return std::make_unique<JuceTreeViewItem>(tree);
}

//--------------------------------------------------------------------------------

std::unique_ptr<juce::Component> vdm::JuceTreeView::Item::createItemComponent(juce::ValueTree tree)
{
    return std::make_unique<JuceTreeViewItemComponent>(tree);
}

//--------------------------------------------------------------------------------

std::unique_ptr<vdm::JuceTreeView::Item> vdm::JuceTreeView::createTreeViewItem(juce::ValueTree tree)
{
    return std::make_unique<vdm::JuceTreeViewItem>(tree);
}

//--------------------------------------------------------------------------------
