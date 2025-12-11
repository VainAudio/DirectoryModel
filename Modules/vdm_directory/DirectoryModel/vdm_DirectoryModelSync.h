#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <juce_core/juce_core.h>

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wextra-semi")
#include <efsw/efsw.hpp>
JUCE_END_IGNORE_WARNINGS_GCC_LIKE

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//-----------------------------------------------------------------------------

class DirectoryModel;

//-----------------------------------------------------------------------------
/**
 * @class DirectoryModelSync
 * @brief synchronize DirectoryModel with a directory on disk
 *
 * This class uses the efsw file watcher to receive file change events from the
 * os. It starts a thread and supports synchronizing multiple DirectoryModels
 * at once. Keeping a static object around might be a good idea if you plan on
 * synchronizing lots of different directories.
 *
 * @see DirectoryModel
 */
class DirectoryModelSync : private efsw::FileWatchListener
{
public:
    DirectoryModelSync();
    ~DirectoryModelSync() override;

    void syncModel(DirectoryModel &model, const juce::FileFilter* fileFilter = nullptr);
    void unsyncModel(DirectoryModel &model);
    bool isModelSynced(DirectoryModel &model);

private:
    void handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename) override;
    void messageThreadHandleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename);

    struct Entry
    {
        DirectoryModel *model;
        const juce::FileFilter* fileFilter;
    };
    using ModelMap = std::map<efsw::WatchID, Entry>;

    ModelMap::iterator findModel(DirectoryModel &model);

    ModelMap m_models;
    efsw::FileWatcher m_fileWatcher;

    JUCE_DECLARE_WEAK_REFERENCEABLE(DirectoryModelSync)
    JUCE_DECLARE_NON_COPYABLE(DirectoryModelSync)
    JUCE_DECLARE_NON_MOVEABLE(DirectoryModelSync)
};

//-----------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
