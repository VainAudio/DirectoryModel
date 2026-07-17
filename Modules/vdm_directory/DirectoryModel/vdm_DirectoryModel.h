#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <juce_data_structures/juce_data_structures.h>
#include "vdm_DirectoryTree.h"

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wextra-semi")
#include <efsw/efsw.hpp>
JUCE_END_IGNORE_WARNINGS_GCC_LIKE

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class DirectoryModel
 * @brief Model a directory on the disk with juce::ValueTree
 *
 * This is the core class of the entire library. It adds some extra properties
 * that support things like directories being open or closed and file types.
 * Custom properties can also be added using IUpdateHandler.
 *
 * Use a DirectoryModelSync to update the DirectoryModel live as changes
 * are made to the file contents on disk.
 *
 * @see IUpdateHandler
 * @see DirectoryModelSync
 */
class DirectoryModel
{
public:
    /**
     * @class IUpdateHandler
     * @brief Add properties to a juce::ValueTree on file adds and changes
     */
    class IUpdateHandler
    {
    public:
        virtual ~IUpdateHandler() = default;

        virtual void onAdd(juce::ValueTree, juce::File) = 0;
        virtual void onModify(juce::ValueTree, juce::File) = 0;
    };

    enum class Type
    {
        None = 0,
        Dir = 1,
        File = 2
    };

    struct Keys
    {
        Keys() = delete;

        static const inline juce::Identifier FileType{ "vdm_file_type" };
        static const inline juce::Identifier DirOpen{ "vdm_dir_open" };
    };

    static bool IsDir(juce::ValueTree tree);
    static bool IsFile(juce::ValueTree tree);
    static bool IsDirOpen(juce::ValueTree tree);
    static void SetDirOpen(juce::ValueTree tree, bool isOpen);
    static void ToggleIsDirOpen(juce::ValueTree tree);

    DirectoryModel() = default;
    ~DirectoryModel();
    DirectoryModel(DirectoryModel &) = delete;
    DirectoryModel(DirectoryModel &&) = delete;
    void operator=(DirectoryModel &) = delete;
    void operator=(DirectoryModel &&) = delete;

    void initialize(const juce::File &root, bool populate = true);
    void initialize(const juce::File &root, const juce::FileFilter *fileFilter);

    juce::ValueTree getValueTree() const;
    juce::File getRootFile() const;

    void addUpdateHandler(IUpdateHandler &updateHandler);

    void addFile(juce::File file);
    void updateFile(juce::File file);
    void removeFile(juce::File file);

private:
    void handleAddUpdate(juce::ValueTree tree, juce::File file) const;
    void handleModifyUpdate(juce::ValueTree tree, juce::File file) const;

    void populateTree(const juce::FileFilter *fileFilter);

    DirectoryTree m_tree;
    std::vector<IUpdateHandler *> m_updateHandlers;

    JUCE_DECLARE_WEAK_REFERENCEABLE(DirectoryModel)
};

//-----------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
