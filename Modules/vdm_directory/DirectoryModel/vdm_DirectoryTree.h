#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <stack>

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class DirectoryTree
 * @brief represents a file system with juce::ValueTrees.
 */
class DirectoryTree
{
public:
    DirectoryTree();
    explicit DirectoryTree(const juce::File &root);

    void initialize(const juce::File &root);
    bool isInitialized() const;

    juce::ValueTree getRootValueTree() const;
    juce::File getRootFile() const;

    bool hasValueTreeAt(const juce::File &file);
    juce::ValueTree getValueTreeAt(const juce::File &file);
    void removeValueTreeAt(const juce::File &file);

private:
    std::stack<juce::String> pathToStack(const juce::File &file) const;
    juce::ValueTree internalGetValueTreeAt(const juce::File &file, bool create);

    juce::File m_root;
    juce::ValueTree m_tree;
};

//-----------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
