#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <juce_data_structures/juce_data_structures.h>

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class DirectoryModelBuilder
 *
 * @brief quickly construct directory models
 */
class DirectoryModelBuilder
{
public:
    explicit DirectoryModelBuilder();
    ~DirectoryModelBuilder();

    DirectoryModelBuilder &withFileNameUpdater();
    DirectoryModelBuilder &withFileSizeUpdater();
    DirectoryModelBuilder &withFileExtensionUpdater();
    DirectoryModelBuilder &withFilePathUpdater();

    std::unique_ptr<DirectoryModel> build();

protected:
    void addHandler(std::unique_ptr<DirectoryModel::IUpdateHandler> &&handler);

private:
    class Impl;
    std::vector<std::unique_ptr<DirectoryModel::IUpdateHandler>> m_updateHandlers{};
};

//-----------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
