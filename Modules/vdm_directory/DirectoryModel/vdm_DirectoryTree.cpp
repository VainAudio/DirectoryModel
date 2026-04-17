#include "vdm_DirectoryTree.h"

//-----------------------------------------------------------------------------

vdm::DirectoryTree::DirectoryTree() = default;

//-----------------------------------------------------------------------------

vdm::DirectoryTree::DirectoryTree(const juce::File &root)
{
    initialize(root);
}

//-----------------------------------------------------------------------------

void vdm::DirectoryTree::initialize(const juce::File &root)
{
    jassert(!isInitialized());                    // don't initialize twice
    jassert(root.exists() && root.isDirectory()); // don't initialize with a nonexistent file

    m_root = root;
    m_tree = juce::ValueTree{ root.getFileName() };
}

//-----------------------------------------------------------------------------

bool vdm::DirectoryTree::isInitialized() const
{
    return m_tree.isValid();
}

//-----------------------------------------------------------------------------

juce::ValueTree vdm::DirectoryTree::getRootValueTree() const
{
    return m_tree;
}

//-----------------------------------------------------------------------------

juce::File vdm::DirectoryTree::getRootFile() const
{
    return m_root;
}

//-----------------------------------------------------------------------------

bool vdm::DirectoryTree::hasValueTreeAt(const juce::File &file)
{
    return internalGetValueTreeAt(file, false).isValid();
}

//-----------------------------------------------------------------------------

juce::ValueTree vdm::DirectoryTree::getValueTreeAt(const juce::File &file)
{
    return internalGetValueTreeAt(file, true);
}

//-----------------------------------------------------------------------------

void vdm::DirectoryTree::removeValueTreeAt(const juce::File &file)
{
    const auto tree{ internalGetValueTreeAt(file, false) };

    if (tree.isValid())
        tree.getParent().removeChild(tree, nullptr);
}

//-----------------------------------------------------------------------------

std::stack<juce::String> vdm::DirectoryTree::pathToStack(const juce::File &f) const
{
    std::stack<juce::String> stack;

    if (m_root != f && !f.isAChildOf(m_root))
    {
        jassertfalse;
        return {};
    }

    juce::File file{ f };
    while (m_root != file && !file.isRoot())
    {
        stack.push(file.getFileName());
        file = file.getParentDirectory();
    }

    return stack;
}

//-----------------------------------------------------------------------------

juce::ValueTree vdm::DirectoryTree::internalGetValueTreeAt(const juce::File &file, bool create)
{
    if (!m_tree.isValid())
    {
        jassertfalse;
        return {};
    }

    auto stack = pathToStack(file);
    auto tree = m_tree;
    while (!stack.empty())
    {
        auto fileName = stack.top();
        auto childTree = tree.getChildWithName(fileName);

        if (create && !childTree.isValid())
        {
            childTree = juce::ValueTree{ fileName };
            tree.addChild(childTree, -1, nullptr);
        }

        tree = childTree;

        stack.pop();
    }

    return tree;
}

//-----------------------------------------------------------------------------
