#include "DirectoryModelDemoJuceTreeViewItem.h"
#include "DirectoryModelDemoJuceTreeViewItemComponent.h"

//--------------------------------------------------------------------------------

DirectoryModelDemoJuceTreeViewItem::DirectoryModelDemoJuceTreeViewItem(juce::ValueTree tree)
{
    setValueTree(tree);
}

//--------------------------------------------------------------------------------

DirectoryModelDemoJuceTreeViewItem::~DirectoryModelDemoJuceTreeViewItem() = default;

//--------------------------------------------------------------------------------

std::unique_ptr<juce::TreeViewItem> DirectoryModelDemoJuceTreeViewItem::createItem(juce::ValueTree tree)
{
    return std::make_unique<DirectoryModelDemoJuceTreeViewItem>(tree);
}

//--------------------------------------------------------------------------------

std::unique_ptr<juce::Component> DirectoryModelDemoJuceTreeViewItem::createItemComponent(juce::ValueTree tree)
{
    return std::make_unique<DirectoryModelDemoJuceTreeViewItemComponent>(tree);
}

//--------------------------------------------------------------------------------
