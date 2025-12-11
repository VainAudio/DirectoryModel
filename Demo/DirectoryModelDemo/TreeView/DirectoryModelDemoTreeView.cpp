#include "DirectoryModelDemoTreeView.h"
#include "DirectoryModelDemoTreeViewItem.h"

//--------------------------------------------------------------------------------

DirectoryModelDemoTreeView::DirectoryModelDemoTreeView()
{
    addKeyListener(&m_dirSelection);
    setWantsKeyboardFocus(true);
}

//--------------------------------------------------------------------------------

DirectoryModelDemoTreeView::~DirectoryModelDemoTreeView()
{
    removeKeyListener(&m_dirSelection);
}

//--------------------------------------------------------------------------------

void DirectoryModelDemoTreeView::setRootItemSelectable(bool b)
{
    m_dirSelection.setRootItemSelectable(b);
}

//--------------------------------------------------------------------------------

void DirectoryModelDemoTreeView::setSelectionValueTree(juce::ValueTree tree)
{
    m_dirSelection.setValueTree(tree);
}

//--------------------------------------------------------------------------------

std::unique_ptr<juce::Component> DirectoryModelDemoTreeView::createTreeViewItem(juce::ValueTree tree)
{
    return std::make_unique<DirectoryModelDemoTreeViewItem>(tree);
}

//--------------------------------------------------------------------------------
