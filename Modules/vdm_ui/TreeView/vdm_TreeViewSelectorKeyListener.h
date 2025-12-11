#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <vdm_directory/vdm_directory.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_data_structures/juce_data_structures.h>

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//--------------------------------------------------------------------------------
/**
 * @class TreeViewSelectorKeyListener
 * @brief Uses SelectionModel to implement tree view selection behavior for a
 * juce::ValueTree.
 *
 * @note SelectionModel is required
 *
 * @see SelectionModel
 */
class TreeViewSelectorKeyListener : public juce::KeyListener
{
public:
    ~TreeViewSelectorKeyListener() override;

    void setValueTree(juce::ValueTree tree);
    void setRootItemSelectable(bool allow);

    bool keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) override;

private:
    void check();

    bool m_allowSelectRootItem{ false };
    TreeViewSelector m_selector;
};

//--------------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
