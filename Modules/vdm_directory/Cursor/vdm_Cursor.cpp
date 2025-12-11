#include "vdm_Cursor.h"

//-----------------------------------------------------------------------------

vdm::Cursor::Cursor() = default;

//-----------------------------------------------------------------------------

vdm::Cursor::Cursor(juce::ValueTree tree)
    : m_tree(tree)
{
}

//-----------------------------------------------------------------------------

bool vdm::Cursor::parent()
{
    if (!m_tree.isValid() || !m_tree.getParent().isValid())
        return false;

    m_tree = m_tree.getParent();
    return true;
}

//-----------------------------------------------------------------------------

bool vdm::Cursor::child()
{
    if (!m_tree.isValid() || m_tree.getNumChildren() == 0)
        return false;

    m_tree = m_tree.getChild(0);
    return true;
}

//-----------------------------------------------------------------------------

bool vdm::Cursor::previous()
{
    return sibling(-1);
}

//-----------------------------------------------------------------------------

bool vdm::Cursor::next()
{
    return sibling(1);
}

//-----------------------------------------------------------------------------

void vdm::Cursor::setValueTree(juce::ValueTree tree)
{
    m_tree = tree;
}

//-----------------------------------------------------------------------------

juce::ValueTree vdm::Cursor::getValueTree() const
{
    return m_tree;
}

//-----------------------------------------------------------------------------

bool vdm::Cursor::sibling(int direction)
{
    if (!m_tree.isValid() || !m_tree.getParent().isValid())
        return false;

    const auto prev{ m_tree.getSibling(direction) };
    if (prev.isValid())
    {
        m_tree = prev;
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
