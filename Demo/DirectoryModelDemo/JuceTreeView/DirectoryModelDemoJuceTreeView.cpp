#include "DirectoryModelDemoJuceTreeView.h"
#include "DirectoryModelDemoJuceTreeViewItem.h"

//--------------------------------------------------------------------------------

std::unique_ptr<vdm::JuceTreeView::Item> DirectoryModelDemoJuceTreeView::createTreeViewItem(juce::ValueTree tree)
{
    return std::make_unique<DirectoryModelDemoJuceTreeViewItem>(tree);
}

//--------------------------------------------------------------------------------
