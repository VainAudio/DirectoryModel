#include "vdm_DirectoryModelUpdateHandlers.h"

//-----------------------------------------------------------------------------

vdm::NameUpdateHandler::~NameUpdateHandler() = default;

//-----------------------------------------------------------------------------

void vdm::NameUpdateHandler::onAdd(juce::ValueTree tree, juce::File file)
{
    tree.setProperty(Key, file.getFileNameWithoutExtension(), nullptr);
}

//-----------------------------------------------------------------------------

void vdm::NameUpdateHandler::onModify(juce::ValueTree, juce::File)
{
}

//-----------------------------------------------------------------------------

vdm::ExtensionUpdateHandler::~ExtensionUpdateHandler() = default;

//-----------------------------------------------------------------------------

void vdm::ExtensionUpdateHandler::onAdd(juce::ValueTree tree, juce::File file)
{
    tree.setProperty(Key, file.getFileExtension().removeCharacters("."), nullptr);
}

//-----------------------------------------------------------------------------

void vdm::ExtensionUpdateHandler::onModify(juce::ValueTree, juce::File)
{
}

//-----------------------------------------------------------------------------

vdm::PathUpdateHandler::~PathUpdateHandler() = default;

//-----------------------------------------------------------------------------

void vdm::PathUpdateHandler::onAdd(juce::ValueTree tree, juce::File file)
{
    tree.setProperty(Key, file.getFullPathName(), nullptr);
}

//-----------------------------------------------------------------------------

void vdm::PathUpdateHandler::onModify(juce::ValueTree, juce::File)
{
}

//-----------------------------------------------------------------------------

vdm::FileSizeUpdateHandler::~FileSizeUpdateHandler()
{
}

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
