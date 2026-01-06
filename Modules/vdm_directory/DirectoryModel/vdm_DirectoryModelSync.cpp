#include "vdm_DirectoryModelSync.h"
#include "vdm_DirectoryModel.h"
#include <ranges>

//-----------------------------------------------------------------------------

vdm::DirectoryModelSync::DirectoryModelSync() = default;
//-----------------------------------------------------------------------------

vdm::DirectoryModelSync::~DirectoryModelSync()
{
    for (const auto &[id, _] : m_models)
        m_fileWatcher.removeWatch(id);
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModelSync::syncModel(DirectoryModel& model, const juce::FileFilter* fileFilter)
{
    if (auto it{ findModel(model) }; it == m_models.end())
    {
        const auto watchID{ m_fileWatcher.addWatch(model.getRootFile().getFullPathName().toStdString(), this, true) };
        m_models[watchID] = { &model, fileFilter };
        m_fileWatcher.watch();
    }
    else
        jassertfalse;
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModelSync::unsyncModel(DirectoryModel& model)
{
    if (const auto it{ findModel(model) }; it != m_models.end())
    {
        m_fileWatcher.removeWatch(it->first);
        m_models.erase(it);
    }
    else
        jassertfalse;
}

//-----------------------------------------------------------------------------

bool vdm::DirectoryModelSync::isModelSynced(DirectoryModel& model)
{
    return findModel(model) != m_models.end();
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModelSync::handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename)
{
    if (const auto it{ m_models.find(watchid) }; it != m_models.end())
    {
        juce::File f{ dir + filename };
        const auto root{ it->second.model->getRootFile() };

        if(f != root && !f.isAChildOf(root))
        {
            // TODO receiving events here for the parent to root - shouldnt be?
            // jassertfalse;
            return;
        }

        auto fn = [=, this, weak = juce::WeakReference{ this }]
        {
            if (!weak.wasObjectDeleted())
            {
                messageThreadHandleFileAction(watchid, dir, filename, action, oldFilename);
            }
        };

        juce::MessageManager::callAsync(fn);
    }
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModelSync::messageThreadHandleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename)
{
    juce::ignoreUnused(watchid, oldFilename);
    jassert(juce::MessageManager::existsAndIsCurrentThread());

    const auto it{ m_models.find(watchid) };

    if (it == m_models.end())
    {
        jassertfalse;
        return;
    }

    auto [model, filter] = it->second;

    const juce::String fullPath{ dir + filename };
    const juce::File file{ fullPath };

    if (filter)
    {
        if (file.isDirectory() && !filter->isDirectorySuitable(file))
            return;
        if (!filter->isFileSuitable(file))
            return;
    }

    switch (action)
    {
    case efsw::Action::Add:
        {
            model->addFile(file);
            break;
        }
    case efsw::Action::Delete:
        {
            model->removeFile(file);
            break;
        }
    case efsw::Action::Modified:
        {
            model->updateFile(file);
            break;
        }
    case efsw::Action::Moved:
        messageThreadHandleFileAction(watchid, dir, oldFilename, efsw::Action::Delete, {});
        messageThreadHandleFileAction(watchid, dir, filename, efsw::Action::Add, {});
        break;
    default:
        jassertfalse;
    }
}

//-----------------------------------------------------------------------------

std::map<efsw::WatchID, vdm::DirectoryModelSync::Entry>::iterator vdm::DirectoryModelSync::findModel(DirectoryModel& model)
{
    return std::ranges::find_if(m_models,[&model](auto pair) { return pair.second.model == &model; });
}

//-----------------------------------------------------------------------------
