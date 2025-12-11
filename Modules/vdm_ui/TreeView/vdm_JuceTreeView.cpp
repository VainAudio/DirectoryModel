#include "vdm_JuceTreeView.h"
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
