#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class TreeViewSelector
 * @brief Arrow key based traversal of a tree view using SelectionModel
 *
 * @note requires SelectionModel
 *
 * @see SelectionModel
 */
class TreeViewSelector
{
public:
    TreeViewSelector();
    explicit TreeViewSelector(juce::ValueTree tree);
    ~TreeViewSelector();

    void keyUp();
    void keyDown();
    void keyLeft();
    void keyRight();

    void setValueTree(juce::ValueTree tree);
    juce::ValueTree getSelectedTree();

private:
    class Impl;
    std::unique_ptr<Impl> m_p;
};

//-----------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
