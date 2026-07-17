#include "vdm_DirectoryModelUpdateHandlers.h"

//-----------------------------------------------------------------------------

vdm::FileNameUpdateHandler::~FileNameUpdateHandler() = default;

//-----------------------------------------------------------------------------

void vdm::FileNameUpdateHandler::onAdd(juce::ValueTree tree, juce::File file)
{
    tree.setProperty(Key, file.getFileNameWithoutExtension(), nullptr);
}

//-----------------------------------------------------------------------------

void vdm::FileNameUpdateHandler::onModify(juce::ValueTree, juce::File)
{
}

//-----------------------------------------------------------------------------

vdm::FileExtensionUpdateHandler::~FileExtensionUpdateHandler() = default;

//-----------------------------------------------------------------------------

void vdm::FileExtensionUpdateHandler::onAdd(juce::ValueTree tree, juce::File file)
{
    tree.setProperty(Key, file.getFileExtension().removeCharacters("."), nullptr);
}

//-----------------------------------------------------------------------------

void vdm::FileExtensionUpdateHandler::onModify(juce::ValueTree, juce::File)
{
}

//-----------------------------------------------------------------------------

vdm::FilePathUpdateHandler::~FilePathUpdateHandler() = default;

//-----------------------------------------------------------------------------

void vdm::FilePathUpdateHandler::onAdd(juce::ValueTree tree, juce::File file)
{
    tree.setProperty(Key, file.getFullPathName(), nullptr);
}

//-----------------------------------------------------------------------------

void vdm::FilePathUpdateHandler::onModify(juce::ValueTree, juce::File)
{
}

//-----------------------------------------------------------------------------

vdm::FileSizeUpdateHandler::~FileSizeUpdateHandler() = default;

//-----------------------------------------------------------------------------

void vdm::FileSizeUpdateHandler::onAdd(juce::ValueTree tree, juce::File file)
{
    tree.setProperty(Key, file.getSize(), nullptr);
}

//-----------------------------------------------------------------------------

void vdm::FileSizeUpdateHandler::onModify(juce::ValueTree tree, juce::File file)
{
    tree.setProperty(Key, file.getSize(), nullptr);
}

//-----------------------------------------------------------------------------
