#include "vdm_DirectoryModel.h"

//-----------------------------------------------------------------------------

bool vdm::DirectoryModel::IsDir(juce::ValueTree tree)
{
    return static_cast<int>(tree.getProperty(Keys::FileType)) == static_cast<int>(Type::Dir);
}

//-----------------------------------------------------------------------------

bool vdm::DirectoryModel::IsFile(juce::ValueTree tree)
{
    return static_cast<int>(tree.getProperty(Keys::FileType)) == static_cast<int>(Type::File);
}

//-----------------------------------------------------------------------------

bool vdm::DirectoryModel::IsDirOpen(juce::ValueTree tree)
{
    return IsDir(tree) && tree.getProperty(Keys::DirOpen);
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModel::SetDirOpen(juce::ValueTree tree, bool isOpen)
{
    if (IsDir(tree))
        tree.setProperty(Keys::DirOpen, isOpen, nullptr);
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModel::ToggleIsDirOpen(juce::ValueTree tree)
{
    SetDirOpen(tree, !IsDirOpen(tree));
}

//-----------------------------------------------------------------------------

vdm::DirectoryModel::~DirectoryModel() = default;

//-----------------------------------------------------------------------------

void vdm::DirectoryModel::initialize(const juce::File &root, bool populate)
{
    if (!m_tree.isInitialized())
    {
        m_tree.initialize(root);
        if (populate)
            populateTree(nullptr);
    }
    else
        jassertfalse;
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModel::initialize(const juce::File &root, const juce::FileFilter *fileFilter)
{
    initialize(root, false);
    populateTree(fileFilter);
}

//-----------------------------------------------------------------------------

juce::ValueTree vdm::DirectoryModel::getValueTree() const
{
    return m_tree.getRootValueTree();
}

//-----------------------------------------------------------------------------

juce::File vdm::DirectoryModel::getRootFile() const
{
    return m_tree.getRootFile();
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModel::addUpdateHandler(IUpdateHandler &updateHandler)
{
    m_updateHandlers.push_back(&updateHandler);
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModel::addFile(juce::File file)
{
    auto tree = m_tree.getValueTreeAt(file);
    handleAddUpdate(tree, file);
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModel::updateFile(juce::File file)
{
    auto tree = m_tree.getValueTreeAt(file);
    handleModifyUpdate(tree, file);
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModel::removeFile(juce::File file)
{
    m_tree.removeValueTreeAt(file);
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModel::handleAddUpdate(juce::ValueTree tree, juce::File file) const
{
    const Type type{ file.isDirectory() ? Type::Dir : Type::File };
    tree.setProperty(Keys::FileType, static_cast<int>(type), nullptr);

    for (auto &i : m_updateHandlers)
        i->onAdd(tree, file);
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModel::handleModifyUpdate(juce::ValueTree tree, juce::File file) const
{
    for (auto &i : m_updateHandlers)
        i->onModify(tree, file);
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModel::populateTree(const juce::FileFilter *filter)
{
    handleAddUpdate(m_tree.getRootValueTree(), m_tree.getRootFile());

    const auto childFiles =
        m_tree.getRootFile().findChildFiles(juce::File::TypesOfFileToFind::findFilesAndDirectories, true);
    for (const auto &child : childFiles)
    {
        auto path = child.getFullPathName().toStdString();
        if (filter)
        {
            if (child.isDirectory() && !filter->isDirectorySuitable(child))
                continue;
            if (!child.isDirectory() && !filter->isFileSuitable(child))
                continue;
        }

        const auto t = m_tree.getValueTreeAt(child);
        handleAddUpdate(t, child);
    }
}

//-----------------------------------------------------------------------------
