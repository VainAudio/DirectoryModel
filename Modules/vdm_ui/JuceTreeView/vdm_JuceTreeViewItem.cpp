#include "vdm_JuceTreeViewItem.h"

//--------------------------------------------------------------------------------

vdm::JuceTreeViewItem::JuceTreeViewItem(juce::ValueTree tree)
{
    setValueTree(tree);
}

//--------------------------------------------------------------------------------

vdm::JuceTreeViewItem::~JuceTreeViewItem() = default;

//--------------------------------------------------------------------------------

std::unique_ptr<juce::TreeViewItem> vdm::JuceTreeViewItem::createItem(juce::ValueTree tree)
{
    return std::make_unique<JuceTreeViewItem>(tree);
}

//--------------------------------------------------------------------------------

std::unique_ptr<juce::Component> vdm::JuceTreeViewItem::createItemComponent(juce::ValueTree tree)
{
    return std::make_unique<JuceTreeViewItemComponent>(tree);
}

//--------------------------------------------------------------------------------
