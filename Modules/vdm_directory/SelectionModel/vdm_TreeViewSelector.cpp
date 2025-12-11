#include "vdm_TreeViewSelector.h"
#include <vdm_directory/vdm_directory.h>

//-----------------------------------------------------------------------------

class vdm::TreeViewSelector::Impl : public vdm::SelectionModelListener
{
public:
    ~Impl() override {}

    void onSelectedTreeChanged(juce::ValueTree tree) override
    {
        if (SelectionModel::IsSelected(tree))
        {
            cursor.setValueTree(tree);
        }
    }

    void selectCurrent() const
    {
        if (cursor.getValueTree().isValid())
            SelectionModel::RequestSelectTree(cursor.getValueTree());
    }

    TreeViewCursor cursor;
};

//-----------------------------------------------------------------------------

vdm::TreeViewSelector::TreeViewSelector()
    : m_p(std::make_unique<Impl>())
{
}

//-----------------------------------------------------------------------------

vdm::TreeViewSelector::TreeViewSelector(juce::ValueTree tree)
    : TreeViewSelector()
{
    setValueTree(tree);
}

//-----------------------------------------------------------------------------

vdm::TreeViewSelector::~TreeViewSelector() = default;

//-----------------------------------------------------------------------------

void vdm::TreeViewSelector::keyUp()
{
    if (m_p->cursor.keyUp())
        m_p->selectCurrent();
}

//-----------------------------------------------------------------------------

void vdm::TreeViewSelector::keyDown()
{
    if (m_p->cursor.keyDown())
        m_p->selectCurrent();
}

//-----------------------------------------------------------------------------

void vdm::TreeViewSelector::keyLeft()
{
    if (m_p->cursor.keyLeft())
        m_p->selectCurrent();
}

//-----------------------------------------------------------------------------

void vdm::TreeViewSelector::keyRight()
{
    if (m_p->cursor.keyRight())
        m_p->selectCurrent();
}

//-----------------------------------------------------------------------------

template<typename Fn>
juce::ValueTree findFirst(juce::ValueTree root, Fn fn)
{
    if (fn(root))
        return root;

    for (auto child : root)
    {
        auto t = findFirst(child, fn);
        if (t.isValid())
            return t;
    }

    return {};
}

//-----------------------------------------------------------------------------

void vdm::TreeViewSelector::setValueTree(juce::ValueTree tree)
{
    m_p->setSelectionModelValueTree(tree);

    auto t = findFirst(tree, [](juce::ValueTree vt) { return vdm::SelectionModel::IsSelected(vt); });
    if (t.isValid())
        m_p->cursor.setValueTree(t);
    else
        m_p->cursor.setValueTree(tree);

    m_p->selectCurrent();
}

//-----------------------------------------------------------------------------

juce::ValueTree vdm::TreeViewSelector::getSelectedTree()
{
    return m_p->cursor.getValueTree();
}

//-----------------------------------------------------------------------------
