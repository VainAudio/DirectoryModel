#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <juce_data_structures/juce_data_structures.h>

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class Cursor
 * @brief Cursor type functionality for traversing juce::ValueTree hierarchies
 *
 * It holds a single value tree and reassigns it based on calls to
 * parent/child/previous/next allowing you to navigate the value tree.
 */
class Cursor
{
public:
    Cursor();
    explicit Cursor(juce::ValueTree tree);

    bool parent();
    bool child();
    bool previous();
    bool next();

    void setValueTree(juce::ValueTree tree);
    juce::ValueTree getValueTree() const;

private:
    bool sibling(int direction);

    juce::ValueTree m_tree;
};

//-----------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
