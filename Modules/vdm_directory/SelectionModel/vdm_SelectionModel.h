#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <span>

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class SelectionModel
 * @brief Adds selection functionality to a juce::ValueTree.
 *
 * Selections are requested so that the existing selection can be unselected.
 * use RequestSelectionKey or RequestSelectTree() to select new trees.
 *
 * Use ISelectionHandler to implement custom selection behavior. This is used
 * to implement shift + click multiselection for example. A selection handler
 * is required. See vdm_SelectionHandlers.h.
 *
 * @see ISelectionHandler
 */
class SelectionModel : private juce::ValueTree::Listener
{
public:
    /**
     * @class ISelectionHandler
     * @brief Create custom selection logic
     * @see vdm_SelectionHandlers.h
     */
    class ISelectionHandler
    {
    public:
        virtual ~ISelectionHandler() = default;
        virtual void selectTree(const std::span<juce::ValueTree> &previouslySelected, juce::ValueTree newSelection) = 0;
    };

    static const inline juce::Identifier ModeKey{ "selection_mode" };
    static const inline juce::Identifier IsSelectedKey{ "is_selected" };
    static const inline juce::Identifier RequestSelectionKey{ "request_selection" };

    static void RequestSelectTree(juce::ValueTree tree, bool select = true);
    static void RequestToggleSelection(juce::ValueTree tree);
    static bool IsSelected(juce::ValueTree tree);
    static void SetSelectionMode(juce::ValueTree tree, const juce::String &selectionMode);

    explicit SelectionModel(juce::ValueTree tree = {});
    ~SelectionModel() override;

    juce::ValueTree getValueTree() const;
    void setValueTree(juce::ValueTree tree);

    void addSelectionHandler(const juce::String &modeId, ISelectionHandler &handler);
    void setSelectionMode(const juce::String &modeId);

    void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
                                  const juce::Identifier &property) override;

private:
    juce::ValueTree m_tree;
    std::map<juce::String, ISelectionHandler *> m_selectionHandlers;
    ISelectionHandler *m_activeSelectionHandler{ nullptr };

    std::vector<juce::ValueTree> m_currentlySelected;
};

//-----------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
