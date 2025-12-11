#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include "vdm_SelectionModel.h"
#include <juce_data_structures/juce_data_structures.h>

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class SelectionModelListener
 * @brief Get notified of new selections.
 *
 * @note requires SelectionModel
 *
 * @see SelectionModel
 */
class SelectionModelListener
{
public:
    SelectionModelListener();
    virtual ~SelectionModelListener();

    void setSelectionModel(SelectionModel &model);
    void setSelectionModelValueTree(juce::ValueTree tree);

    virtual void onSelectedTreeChanged(juce::ValueTree tree) = 0;

private:
    class Impl;
    std::unique_ptr<Impl> m_p;
};

//-----------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
