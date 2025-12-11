#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_data_structures/juce_data_structures.h>
#include "vdm_JuceTreeViewItem.h"

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//--------------------------------------------------------------------------------
/**
 * @class JuceTreeView
 * @brief Integrates a DirectoryModel value tree into a juce::TreeView.
 *
 * This whole class is kind of annoying. It's only really useful if your code is
 * tightly coupled to a juce::TreeView and you need it to work with a
 * DirectoryModel / SelectionModel. Otherwise, prefer TreeView.
 *
 * @see JuceTreeViewItem
 */
class JuceTreeView : public juce::TreeView
{
public:
    ~JuceTreeView() override;

    void setValueTree(juce::ValueTree tree);

private:
    virtual std::unique_ptr<JuceTreeViewItem> createTreeViewItem(juce::ValueTree tree) = 0;
    std::unique_ptr<JuceTreeViewItem> m_p;
};

//--------------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
