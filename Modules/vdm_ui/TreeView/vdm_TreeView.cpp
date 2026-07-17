#include "vdm_TreeView.h"
#include <vdm_directory/vdm_directory.h>

//--------------------------------------------------------------------------------

void vdm::TreeView::setValueTree(juce::ValueTree tree)
{
    m_root.initialize(tree, [this](juce::ValueTree t) { return internalCreateTreeViewItem(t); });

    m_root.tree.addListener(this);
}

//--------------------------------------------------------------------------------

void vdm::TreeView::setRootItemVisible(bool showRootFolder)
{
    m_showRootFolder = showRootFolder;
    resized();
}

//--------------------------------------------------------------------------------

void vdm::TreeView::setSubFolderIndentation(int pixels)
{
    m_subFolderIndentation = pixels;
    resized();
}

//--------------------------------------------------------------------------------

void vdm::TreeView::setItemMargin(int pixels)
{
    m_itemMargin = pixels;
    resized();
}

//--------------------------------------------------------------------------------

void vdm::TreeView::setItemHeight(int pixels)
{
    m_itemHeight = pixels;
    resized();
}

//--------------------------------------------------------------------------------

void vdm::TreeView::resized()
{
    auto b{ getLocalBounds() };
    if (m_showRootFolder)
        m_root.setBounds(b, m_subFolderIndentation, m_itemMargin, m_itemHeight);
    else
    {
        if (m_root.component)
            m_root.component->setBounds({});

        for (auto &node : m_root.subNodes)
            node.setBounds(b, m_subFolderIndentation, m_itemMargin, m_itemHeight);
    }
}

//--------------------------------------------------------------------------------

int vdm::TreeView::getTotalHeight() const
{
    return m_root.getHeight(m_itemHeight, m_itemMargin);
}

//--------------------------------------------------------------------------------

int vdm::TreeView::getMaxIndentLevel() const
{
    return m_root.getMaxIndentLevel();
}

//--------------------------------------------------------------------------------

void vdm::TreeView::valueTreeChildAdded(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded)
{
    if (auto node = getNode(parentTree))
    {
        const int index{ parentTree.indexOf(childWhichHasBeenAdded) };
        Node newNode{
            .component = createTreeViewItem(childWhichHasBeenAdded),
            .tree = childWhichHasBeenAdded,
            .subNodes = {},
        };

        addAndMakeVisible(*newNode.component);
        node->subNodes.emplace(node->subNodes.begin() + index, std::move(newNode));
    }

    resized();
}

//--------------------------------------------------------------------------------

void vdm::TreeView::valueTreeChildRemoved(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenRemoved,
                                          int indexFromWhichChildWasRemoved)
{
    juce::ignoreUnused(childWhichHasBeenRemoved);
    if (auto node = getNode(parentTree))
    {
        node->subNodes.erase(node->subNodes.begin() + indexFromWhichChildWasRemoved);
    }

    resized();
}

//--------------------------------------------------------------------------------

void vdm::TreeView::valueTreeChildOrderChanged(juce::ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex,
                                               int newIndex)
{
    if (auto node = getNode(parentTreeWhoseChildrenHaveMoved))
    {
        auto moveNode{ std::move(node->subNodes[static_cast<std::size_t>(oldIndex)]) };
        node->subNodes.erase(node->subNodes.begin() + oldIndex);
        node->subNodes.emplace(node->subNodes.begin() + newIndex, std::move(moveNode));
    }

    resized();
}

//--------------------------------------------------------------------------------

void vdm::TreeView::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
                                             const juce::Identifier &property)
{
    juce::ignoreUnused(treeWhosePropertyHasChanged);

    if (property == DirectoryModel::Keys::DirOpen)
    {
        const auto node = getNode(treeWhosePropertyHasChanged);
        if (node && DirectoryModel::IsDirOpen(treeWhosePropertyHasChanged))
            node->initialize(treeWhosePropertyHasChanged,
                             [this](juce::ValueTree t) { return internalCreateTreeViewItem(t); });

        resized();
    }
}

//--------------------------------------------------------------------------------

int vdm::TreeView::Node::getMaxIndentLevel() const
{
    if (DirectoryModel::IsDirOpen(tree))
    {
        int max{ 0 };
        for (auto &sub : subNodes)
            max = std::max(max, sub.getMaxIndentLevel());
        return 1 + max;
    }

    return 0;
}

//--------------------------------------------------------------------------------

int vdm::TreeView::Node::getHeight(int itemHeight, int margin) const
{
    int h{ itemHeight };
    h += margin;

    if (DirectoryModel::IsDirOpen(tree))
    {
        for (auto &sub : subNodes)
        {
            h += sub.getHeight(itemHeight, margin);
        }
    }

    return h;
}

//--------------------------------------------------------------------------------

void vdm::TreeView::Node::setBounds(juce::Rectangle<int> &bounds, int indent, int margin, int height)
{
    if (component)
        component->setBounds(bounds.removeFromTop(height));

    bounds.removeFromTop(margin);

    if (DirectoryModel::IsDirOpen(tree))
    {
        bounds.removeFromLeft(indent);

        for (auto &subNode : subNodes)
            subNode.setBounds(bounds, indent, margin, height);

        bounds.setWidth(bounds.getWidth() + indent);
        bounds.translate(-indent, 0);
    }
    else
    {
        for (auto &subNode : subNodes)
        {
            juce::Rectangle<int> r;
            subNode.setBounds(r, indent, margin, height);
        }
    }
}

//--------------------------------------------------------------------------------

void vdm::TreeView::Node::initialize(juce::ValueTree t,
                                     const std::function<std::unique_ptr<juce::Component>(juce::ValueTree)> &fn)
{
    tree = t;

    if (!component)
        component = fn(t);

    if (DirectoryModel::IsDirOpen(t) && subNodes.empty())
    {
        for (const auto child : tree)
        {
            Node subNode;
            subNode.initialize(child, fn);
            subNodes.push_back(std::move(subNode));
        }
    }
}

//--------------------------------------------------------------------------------

vdm::TreeView::Node *vdm::TreeView::getNode(juce::ValueTree tree)
{
    std::stack<juce::Identifier> path;
    for (auto t = tree; t.isValid(); t = t.getParent())
        path.emplace(t.getType());

    if (path.top() == m_root.tree.getType())
    {
        path.pop();
        Node *node{ &m_root };

        while (!path.empty() && node)
        {
            const auto it =
                std::ranges::find_if(node->subNodes, [&path](auto &item) { return item.tree.getType() == path.top(); });
            if (it != node->subNodes.end())
            {
                node = &*it;
                path.pop();
            }
            else
            {
                jassertfalse;
                node = nullptr;
                break;
            }
        }

        return node;
    }

    jassertfalse;
    return nullptr;
}

//--------------------------------------------------------------------------------

std::unique_ptr<juce::Component> vdm::TreeView::internalCreateTreeViewItem(juce::ValueTree tree)
{
    auto ptr{ createTreeViewItem(tree) };

    if (ptr)
        addAndMakeVisible(*ptr);

    return ptr;
}

//--------------------------------------------------------------------------------
