#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include "vdm_Cursor.h"
#include <juce_data_structures/juce_data_structures.h>

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class TreeViewCursor
 * @brief Tree view cursor functionality for juce::ValueTree
 *
 * These make sense when you're looking at a classic tree view.
 * keyUp/Down/Left/Right correspond to what you'd expect from the arrow keys.
 */
class TreeViewCursor
{
public:
    TreeViewCursor();
    explicit TreeViewCursor(juce::ValueTree tree);

    bool keyUp();
    bool keyDown();
    bool keyLeft();
    bool keyRight();

    void setValueTree(juce::ValueTree tree);
    juce::ValueTree getValueTree() const;

private:
    Cursor m_cursor;
};

//-----------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
