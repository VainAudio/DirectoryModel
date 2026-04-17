#include "vdm_SelectionModel.h"

//-----------------------------------------------------------------------------

void vdm::SelectionModel::RequestSelectTree(juce::ValueTree tree, bool select)
{
    tree.setProperty(RequestSelectionKey, select, nullptr);
}

//-----------------------------------------------------------------------------

bool vdm::SelectionModel::IsSelected(juce::ValueTree tree)
{
    return tree.getProperty(IsSelectedKey);
}

//-----------------------------------------------------------------------------

void vdm::SelectionModel::RequestToggleSelection(juce::ValueTree tree)
{
    RequestSelectTree(tree, !IsSelected(tree));
}

//-----------------------------------------------------------------------------

void vdm::SelectionModel::SetSelectionMode(juce::ValueTree tree, const juce::String &selectionMode)
{
    tree.setProperty(ModeKey, selectionMode, nullptr);
}

//-----------------------------------------------------------------------------

vdm::SelectionModel::SelectionModel(juce::ValueTree tree)
{
    setValueTree(tree);
}

//-----------------------------------------------------------------------------

vdm::SelectionModel::~SelectionModel()
{
    m_tree.removeListener(this);
}

//-----------------------------------------------------------------------------

juce::ValueTree vdm::SelectionModel::getValueTree() const
{
    return m_tree;
}

//-----------------------------------------------------------------------------

void vdm::SelectionModel::setValueTree(juce::ValueTree tree)
{
    if (m_tree.isValid())
        m_tree.removeListener(this);

    m_tree = tree;
    m_tree.addListener(this);
}

//-----------------------------------------------------------------------------

void vdm::SelectionModel::addSelectionHandler(const juce::String &modeId, ISelectionHandler &handler)
{
    m_selectionHandlers[modeId] = &handler;
}

//-----------------------------------------------------------------------------

void vdm::SelectionModel::setSelectionMode(const juce::String &modeId)
{
    SetSelectionMode(m_tree, modeId);
}

//-----------------------------------------------------------------------------

void vdm::SelectionModel::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
                                                   const juce::Identifier &property)
{
    if (property == RequestSelectionKey && treeWhosePropertyHasChanged.hasProperty(RequestSelectionKey))
    {
        if (m_activeSelectionHandler)
        {
            auto todo = m_currentlySelected;
            m_activeSelectionHandler->selectTree(todo, treeWhosePropertyHasChanged);
            treeWhosePropertyHasChanged.removeProperty(RequestSelectionKey, nullptr);
        }
        else
            jassertfalse;
    }
    if (property == IsSelectedKey)
    {
        if (treeWhosePropertyHasChanged.getProperty(IsSelectedKey))
            m_currentlySelected.push_back(treeWhosePropertyHasChanged);
        else
        {
            const auto it{ std::ranges::find_if(m_currentlySelected, [treeWhosePropertyHasChanged](auto t)
                                                { return t == treeWhosePropertyHasChanged; }) };

            if (it != m_currentlySelected.end())
                m_currentlySelected.erase(it);
        }
    }
    if (property == ModeKey)
    {
        if (const auto it = m_selectionHandlers.find(treeWhosePropertyHasChanged.getProperty(property));
            it != m_selectionHandlers.end())
            m_activeSelectionHandler = it->second;
        else
            jassertfalse;
    }
}

//-----------------------------------------------------------------------------
