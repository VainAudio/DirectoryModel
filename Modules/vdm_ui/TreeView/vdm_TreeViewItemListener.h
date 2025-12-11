#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_data_structures/juce_data_structures.h>

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//--------------------------------------------------------------------------------
/**
 * @class TreeViewItemListener
 * @brief Get callbacks related to tree view item properties
 *
 * @note This reacts to DirectoryModel keys for openness and SelectionModel keys
 * for selectedness. DirectoryModel is not required.

 * @note requires SelectionModel for onItemSelectedChanged callbacks
 */
class TreeViewItemListener
{
public:
    explicit TreeViewItemListener(juce::ValueTree tree);
    virtual ~TreeViewItemListener();

    virtual void onItemSelectedChanged(bool isSelected) = 0;
    virtual void onItemIsDirOpenChanged(bool isDirOpen) = 0;

protected:
    void initTreeViewItemListener();

private:
    class Impl;
    std::unique_ptr<Impl> m_p;
};

//--------------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
