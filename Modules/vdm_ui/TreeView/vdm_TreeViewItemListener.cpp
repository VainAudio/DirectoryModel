#include "vdm_TreeViewItemListener.h"
#include <vdm_directory/vdm_directory.h>

//--------------------------------------------------------------------------------

class vdm::TreeViewItemListener::Impl : public juce::ValueTree::Listener
{
public:
    Impl(TreeViewItemListener &i, juce::ValueTree t)
        : item(i)
        , tree(t)
    {
        tree.addListener(this);
    }

    ~Impl() override
    {
        tree.removeListener(this);
    }

    void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
                                  const juce::Identifier &property) override
    {
        if (tree == treeWhosePropertyHasChanged)
        {
            if (property == DirectoryModel::Keys::DirOpen)
                item.onItemIsDirOpenChanged(tree.getProperty(DirectoryModel::Keys::DirOpen));
            else if (property == SelectionModel::IsSelectedKey)
                item.onItemSelectedChanged(tree.getProperty(SelectionModel::IsSelectedKey));
        }
    }

    TreeViewItemListener &item;
    juce::ValueTree tree;
};

//--------------------------------------------------------------------------------

vdm::TreeViewItemListener::TreeViewItemListener(juce::ValueTree tree)
    : m_p(std::make_unique<Impl>(*this, tree))
{
}

//--------------------------------------------------------------------------------

vdm::TreeViewItemListener::~TreeViewItemListener() = default;

//--------------------------------------------------------------------------------

void vdm::TreeViewItemListener::initTreeViewItemListener()
{
    for (int i = 0; i < m_p->tree.getNumProperties(); i++)
    {
        const auto propId{ m_p->tree.getPropertyName(i) };
        m_p->valueTreePropertyChanged(m_p->tree, propId);
    }
}

//--------------------------------------------------------------------------------
