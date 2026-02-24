#include "vdm_TreeViewCursor.h"
#include <vdm_directory/vdm_directory.h>

//-----------------------------------------------------------------------------

vdm::TreeViewCursor::TreeViewCursor() = default;

//-----------------------------------------------------------------------------

vdm::TreeViewCursor::TreeViewCursor(juce::ValueTree tree)
    : m_cursor(tree)
{
}

//-----------------------------------------------------------------------------

bool vdm::TreeViewCursor::keyUp()
{
    if (m_cursor.previous())
    {
        while (DirectoryModel::IsDirOpen(m_cursor.getValueTree()))
        {
            if (m_cursor.child())
            {
                while (m_cursor.next()) {}
            }
            else
            {
                m_cursor.previous();
            }
        }
        return true;
    }
    if (m_cursor.parent())
        return true;

    return false;
}

//-----------------------------------------------------------------------------

bool vdm::TreeViewCursor::keyDown()
{
    if (DirectoryModel::IsDirOpen(m_cursor.getValueTree()))
    {
        if (!m_cursor.child())
        {
            m_cursor.next();
        }
    }
    else if (m_cursor.next()) {}
    else
    {
        while (m_cursor.parent() && !m_cursor.next()) {}
    }

    return true;
}

//-----------------------------------------------------------------------------

bool vdm::TreeViewCursor::keyLeft()
{
    auto cursorTree = m_cursor.getValueTree();

    if (DirectoryModel::IsDir(cursorTree) && DirectoryModel::IsDirOpen(cursorTree))
        DirectoryModel::SetDirOpen(cursorTree, false);
    else if (m_cursor.parent())
        return true;

    return false;
}

//-----------------------------------------------------------------------------

bool vdm::TreeViewCursor::keyRight()
{
    if (DirectoryModel::IsDir(m_cursor.getValueTree()))
    {
        if (!DirectoryModel::IsDirOpen(m_cursor.getValueTree()))
            DirectoryModel::SetDirOpen(m_cursor.getValueTree(), true);
        else
        {
            m_cursor.child();
            return true;
        }
    }
    else
    {
        m_cursor.next();
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------

void vdm::TreeViewCursor::setValueTree(juce::ValueTree tree)
{
    m_cursor.setValueTree(tree);
}

//-----------------------------------------------------------------------------

juce::ValueTree vdm::TreeViewCursor::getValueTree() const
{
    return m_cursor.getValueTree();
}

//-----------------------------------------------------------------------------
