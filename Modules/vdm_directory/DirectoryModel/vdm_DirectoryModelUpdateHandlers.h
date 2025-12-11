#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include "vdm_DirectoryModel.h"

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class NameUpdateHandler
 * @brief set the file name without extension (file.txt -> file)
 */
class NameUpdateHandler final : public DirectoryModel::IUpdateHandler
{
public:
    static const inline juce::Identifier Key{ "name" };

    ~NameUpdateHandler() override;

    void onAdd(juce::ValueTree, juce::File) override;
    void onModify(juce::ValueTree, juce::File) override;
};

//-----------------------------------------------------------------------------
/**
 * @class ExtensionUpdateHandler
 * @brief set the file extension (file.txt -> txt)
 */
class ExtensionUpdateHandler final : public DirectoryModel::IUpdateHandler
{
public:
    static const inline juce::Identifier Key{ "extension" };

    ~ExtensionUpdateHandler() override;

    void onAdd(juce::ValueTree, juce::File) override;
    void onModify(juce::ValueTree, juce::File) override;
};

//-----------------------------------------------------------------------------
/**
 * @class PathUpdateHandler
 * @brief set the full file path
 */
class PathUpdateHandler final : public DirectoryModel::IUpdateHandler
{
public:
    static const inline juce::Identifier Key{ "path" };

    ~PathUpdateHandler() override;

    void onAdd(juce::ValueTree, juce::File) override;
    void onModify(juce::ValueTree, juce::File) override;
};

//-----------------------------------------------------------------------------
/**
 * @class FileSizeUpdateHandler
 * @brief set the file size in bytes
 */
class FileSizeUpdateHandler final : public DirectoryModel::IUpdateHandler
{
public:
    static const inline juce::Identifier Key{ "file_size" };

    ~FileSizeUpdateHandler() override;

    void onAdd(juce::ValueTree, juce::File) override;
    void onModify(juce::ValueTree, juce::File) override;
};

//-----------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
