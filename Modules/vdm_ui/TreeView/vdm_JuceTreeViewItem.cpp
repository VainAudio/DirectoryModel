#include "vdm_JuceTreeViewItem.h"
#include <vdm_directory/vdm_directory.h>

//--------------------------------------------------------------------------------

vdm::JuceTreeViewItem::JuceTreeViewItem() = default;

//--------------------------------------------------------------------------------

vdm::JuceTreeViewItem::~JuceTreeViewItem()
{
    m_tree.removeListener(this);
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeViewItem::setValueTree(juce::ValueTree tree)
{
    clearSubItems();

    m_tree = tree;
    m_tree.addListener(this);
    for (int i = 0; i < tree.getNumChildren(); i++)
    {
        auto child = tree.getChild(i);
        vdm::JuceTreeViewItem::valueTreeChildAdded(tree, child);
    }
    if (vdm::DirectoryModel::IsDirOpen(tree))
        setOpen(true);
    if (vdm::SelectionModel::IsSelected(tree))
        setSelected(true, false, juce::NotificationType::dontSendNotification);
}

//--------------------------------------------------------------------------------

bool vdm::JuceTreeViewItem::mightContainSubItems()
{
    return vdm::DirectoryModel::IsDir(m_tree);
    return m_tree.getNumChildren() > 0;
}

//--------------------------------------------------------------------------------

std::unique_ptr<juce::Component> vdm::JuceTreeViewItem::createItemComponent()
{
    return createItemComponent(m_tree);
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeViewItem::itemOpennessChanged(bool isNowOpen)
{
    if (vdm::DirectoryModel::IsDir(m_tree))
        vdm::DirectoryModel::SetDirOpen(m_tree, isNowOpen);
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeViewItem::itemSelectionChanged(bool isNowSelected)
{
    vdm::SelectionModel::RequestSelectTree(m_tree, isNowSelected);
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeViewItem::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    if (treeWhosePropertyHasChanged == m_tree)
    {
        if (property == vdm::DirectoryModel::DirOpenKey)
            setOpen(vdm::DirectoryModel::IsDirOpen(m_tree));
        if (property == vdm::SelectionModel::IsSelectedKey)
            setSelected(vdm::SelectionModel::IsSelected(m_tree), false, juce::NotificationType::dontSendNotification);
    }
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeViewItem::valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded)
{
    auto path = childWhichHasBeenAdded.getProperty(PathUpdateHandler::Key).toString().toStdString();
    if (parentTree == m_tree)
    {
        auto ptr = createItem(childWhichHasBeenAdded);
        addSubItem(ptr.release());
    }
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeViewItem::valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved)
{
    juce::ignoreUnused(childWhichHasBeenRemoved);

    if (parentTree == m_tree)
    {
        removeSubItem(indexFromWhichChildWasRemoved);
    }
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeViewItem::valueTreeChildOrderChanged(juce::ValueTree& parentTree, int oldIndex, int newIndex)
{
    if (parentTree == m_tree)
    {
        const auto item{ getSubItem(oldIndex) };
        removeSubItem(oldIndex, false);
        addSubItem(item, newIndex);
    }
}

//--------------------------------------------------------------------------------
