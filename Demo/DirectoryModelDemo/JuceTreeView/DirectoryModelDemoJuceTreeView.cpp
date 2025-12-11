#include "DirectoryModelDemoJuceTreeView.h"
#include "DirectoryModelDemoJuceTreeViewItem.h"

//--------------------------------------------------------------------------------

std::unique_ptr<vdm::JuceTreeViewItem> DirectoryModelDemoJuceTreeView::createTreeViewItem(juce::ValueTree tree)
{
    return std::make_unique<DirectoryModelDemoJuceTreeViewItem>(tree);
}

//--------------------------------------------------------------------------------
