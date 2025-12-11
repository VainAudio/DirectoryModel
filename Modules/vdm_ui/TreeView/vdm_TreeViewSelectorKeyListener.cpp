#include "vdm_TreeViewSelectorKeyListener.h"

//--------------------------------------------------------------------------------

vdm::TreeViewSelectorKeyListener::~TreeViewSelectorKeyListener() = default;

//--------------------------------------------------------------------------------

void vdm::TreeViewSelectorKeyListener::setValueTree(juce::ValueTree tree)
{
    m_selector.setValueTree(tree);
}

//--------------------------------------------------------------------------------

void vdm::TreeViewSelectorKeyListener::setRootItemSelectable(bool allow)
{
    m_allowSelectRootItem = allow;
    check();
}

//--------------------------------------------------------------------------------

bool vdm::TreeViewSelectorKeyListener::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent)
{
    juce::ignoreUnused(originatingComponent);

    if (key.isKeyCode(juce::KeyPress::upKey))
        m_selector.keyUp();
    else if (key.isKeyCode(juce::KeyPress::rightKey))
        m_selector.keyRight();
    else if (key.isKeyCode(juce::KeyPress::downKey))
        m_selector.keyDown();
    else if (key.isKeyCode(juce::KeyPress::leftKey))
        m_selector.keyLeft();
    else
        return false;

    check();

    return true;
}

//--------------------------------------------------------------------------------

void vdm::TreeViewSelectorKeyListener::check()
{
    if (!m_allowSelectRootItem)
    {
        if (!m_selector.getSelectedTree().getParent().isValid())
            m_selector.keyDown();
    }
}

//--------------------------------------------------------------------------------
