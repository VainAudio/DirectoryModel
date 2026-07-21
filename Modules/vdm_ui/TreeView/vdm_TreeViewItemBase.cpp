#include "vdm_TreeViewItemBase.h"
#include <vdm_directory/vdm_directory.h>

//--------------------------------------------------------------------------------

class vdm::TreeViewItemBase::Impl : public juce::ValueTree::Listener
{
public:
    Impl(TreeViewItemBase &i, juce::ValueTree t)
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

    TreeViewItemBase &item;
    juce::ValueTree tree;
};

//--------------------------------------------------------------------------------

vdm::TreeViewItemBase::TreeViewItemBase(juce::ValueTree tree)
    : m_p(std::make_unique<Impl>(*this, tree))
{
}

//--------------------------------------------------------------------------------

vdm::TreeViewItemBase::~TreeViewItemBase() = default;

//--------------------------------------------------------------------------------

void vdm::TreeViewItemBase::initTreeViewItemListener()
{
    for (int i = 0; i < m_p->tree.getNumProperties(); i++)
    {
        const auto propId{ m_p->tree.getPropertyName(i) };
        m_p->valueTreePropertyChanged(m_p->tree, propId);
    }
}

//--------------------------------------------------------------------------------
