#include "vdm_SelectionHandlers.h"
#include <vdm_directory/vdm_directory.h>
#include <ranges>

//-----------------------------------------------------------------------------

vdm::SingleSelectionHandler::~SingleSelectionHandler() = default;

//-----------------------------------------------------------------------------

void vdm::SingleSelectionHandler::selectTree(const std::span<juce::ValueTree>& previouslySelected, juce::ValueTree newSelection)
{
    for (auto t : previouslySelected)
        t.setProperty(SelectionModel::IsSelectedKey, false, nullptr);

    newSelection.setProperty(SelectionModel::IsSelectedKey, true, nullptr);
}

//-----------------------------------------------------------------------------

vdm::PathSelectionHandler::~PathSelectionHandler() = default;

//-----------------------------------------------------------------------------

void vdm::PathSelectionHandler::selectTree(const std::span<juce::ValueTree>& previouslySelected, juce::ValueTree newSelection)
{
    std::stack<juce::ValueTree> newPath;
    auto t = newSelection;
    do
    {
        newPath.push(t);
        t = t.getParent();
    } while (t.isValid());

    for (auto i : previouslySelected)
    {
        if (!newPath.empty() && i == newPath.top())
            newPath.pop();
        else
            i.setProperty(SelectionModel::IsSelectedKey, false, nullptr);
    }

    while (!newPath.empty())
    {
        newPath.top().setProperty(SelectionModel::IsSelectedKey, true, nullptr);
        newPath.pop();
    }
}

//-----------------------------------------------------------------------------

vdm::GroupMultiSelectionHandler::~GroupMultiSelectionHandler() = default;

//-----------------------------------------------------------------------------

template<typename Fn>
juce::ValueTree FindFirst(juce::ValueTree root, const Fn &fn)
{
    if (fn(root))
        return root;

    for (const auto &child : root)
    {
        auto v = FindFirst(child, fn);
        if (v.isValid())
            return v;
    }

    return {};
}

//-----------------------------------------------------------------------------

void vdm::GroupMultiSelectionHandler::selectTree(const std::span<juce::ValueTree>& previouslySelected, juce::ValueTree newSelection)
{
    if (previouslySelected.empty())
    {
        newSelection.setProperty(SelectionModel::IsSelectedKey, true, nullptr);
        return;
    }

    std::ranges::reverse_view rv{ previouslySelected | std::views::drop(1) };
    for (auto t : rv)
    {
        t.setProperty(SelectionModel::IsSelectedKey, false, nullptr);
    }

    const auto from{ previouslySelected.front() };
    const auto to{ newSelection };

    auto root = [to]()
    {
        auto t = to;

        while (t.getParent().isValid())
            t = t.getParent();

        return t;
    }();

    const auto first{ FindFirst(root, [to, from](auto vt) { return vt == to || vt == from; })};

    vdm::TreeViewCursor dir{ from };
    while (to != dir.getValueTree())
    {
        const bool selectedNextTree{ first == from ? dir.keyDown() : dir.keyUp() };
        if (!selectedNextTree)
            break;

        dir.getValueTree().setProperty(SelectionModel::IsSelectedKey, true, nullptr);
    }
}

//-----------------------------------------------------------------------------

vdm::IndividualMultiSelectionHandler::~IndividualMultiSelectionHandler() = default;

//-----------------------------------------------------------------------------

void vdm::IndividualMultiSelectionHandler::selectTree(const std::span<juce::ValueTree>& previouslySelected, juce::ValueTree newSelection)
{
    const bool isAlreadySelected{ std::ranges::find_if(previouslySelected, [newSelection](auto t){ return t == newSelection; }) != previouslySelected.end() };
    newSelection.setProperty(SelectionModel::IsSelectedKey, !isAlreadySelected, nullptr);
}

//-----------------------------------------------------------------------------
