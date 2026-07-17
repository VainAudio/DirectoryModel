#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include "vdm_DirectoryModel.h"

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class FileNameUpdateHandler
 * @brief set the file name without extension (file.txt -> file)
 */
class FileNameUpdateHandler final : public DirectoryModel::IUpdateHandler
{
public:
    static const inline juce::Identifier Key{ "vdm_file_name" };

    ~FileNameUpdateHandler() override;

    void onAdd(juce::ValueTree, juce::File) override;
    void onModify(juce::ValueTree, juce::File) override;
};

//-----------------------------------------------------------------------------
/**
 * @class FileExtensionUpdateHandler
 * @brief set the file extension (file.txt -> txt)
 */
class FileExtensionUpdateHandler final : public DirectoryModel::IUpdateHandler
{
public:
    static const inline juce::Identifier Key{ "vdm_file_extension" };

    ~FileExtensionUpdateHandler() override;

    void onAdd(juce::ValueTree, juce::File) override;
    void onModify(juce::ValueTree, juce::File) override;
};

//-----------------------------------------------------------------------------
/**
 * @class FilePathUpdateHandler
 * @brief set the full file path
 */
class FilePathUpdateHandler final : public DirectoryModel::IUpdateHandler
{
public:
    static const inline juce::Identifier Key{ "vdm_file_path" };

    ~FilePathUpdateHandler() override;

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
    static const inline juce::Identifier Key{ "vdm_file_size" };

    ~FileSizeUpdateHandler() override;

    void onAdd(juce::ValueTree, juce::File) override;
    void onModify(juce::ValueTree, juce::File) override;
};

//-----------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
