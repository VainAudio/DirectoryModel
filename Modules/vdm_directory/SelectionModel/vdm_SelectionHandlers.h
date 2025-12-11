#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include "vdm_SelectionModel.h"

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class SingleSelectionHandler
 * @brief Select the new tree, deselect anything that is already selected.
 */
class SingleSelectionHandler final : public SelectionModel::ISelectionHandler
{
public:
    constexpr static const char *Mode{ "single" };

    ~SingleSelectionHandler() override;

    void selectTree(const std::span<juce::ValueTree>& previouslySelected, juce::ValueTree newSelection) override;
};

//-----------------------------------------------------------------------------
/**
 * @class SingleSelectionHandler
 * @brief Select all of the trees in the path to the current selection
 */
class PathSelectionHandler final : public SelectionModel::ISelectionHandler
{
public:
    constexpr static const char *Mode{ "path" };

    ~PathSelectionHandler() override;

    void selectTree(const std::span<juce::ValueTree>& previouslySelected, juce::ValueTree newSelection) override;
};

//-----------------------------------------------------------------------------
/**
 * @class SingleSelectionHandler
 * @brief Shift + click selection
 */
class GroupMultiSelectionHandler final : public SelectionModel::ISelectionHandler
{
public:
    constexpr static const char *Mode{ "group_multi" };

    ~GroupMultiSelectionHandler() override;

    void selectTree(const std::span<juce::ValueTree>& previouslySelected, juce::ValueTree newSelection) override;
};

//-----------------------------------------------------------------------------
/**
 * @class SingleSelectionHandler
 * @brief Ctrl + click selection
 */
class IndividualMultiSelectionHandler final : public SelectionModel::ISelectionHandler
{
public:
    constexpr static const char *Mode{ "individual_multi" };

    ~IndividualMultiSelectionHandler() override;

    void selectTree(const std::span<juce::ValueTree>& previouslySelected, juce::ValueTree newSelection) override;
};


END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
