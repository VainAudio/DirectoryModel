#include "DirectoryModelDemoTreeView.h"

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
