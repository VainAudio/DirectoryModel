#include "vdm_DirectoryModelBuilder.h"
#include "vdm_DirectoryModelUpdateHandlers.h"

//-----------------------------------------------------------------------------

class vdm::DirectoryModelBuilder::Impl : public vdm::DirectoryModel
{
public:
    explicit Impl(std::vector<std::unique_ptr<DirectoryModel::IUpdateHandler>> &&updateHandlers)
        : m_updateHandlers(std::move(updateHandlers))
    {
        for (const auto &u : m_updateHandlers)
            addUpdateHandler(*u);
    }

private:
    std::vector<std::unique_ptr<DirectoryModel::IUpdateHandler>> m_updateHandlers;
};

//-----------------------------------------------------------------------------

vdm::DirectoryModelBuilder::DirectoryModelBuilder() = default;

//-----------------------------------------------------------------------------

vdm::DirectoryModelBuilder::~DirectoryModelBuilder()
{
    jassert(m_updateHandlers.empty());
}

//-----------------------------------------------------------------------------

vdm::DirectoryModelBuilder &vdm::DirectoryModelBuilder::withFileSizeUpdater()
{
    addHandler(std::make_unique<vdm::FileSizeUpdateHandler>());
    return *this;
}

//-----------------------------------------------------------------------------

vdm::DirectoryModelBuilder &vdm::DirectoryModelBuilder::withFileExtensionUpdater()
{
    addHandler(std::make_unique<vdm::FileExtensionUpdateHandler>());
    return *this;
}

//-----------------------------------------------------------------------------

vdm::DirectoryModelBuilder &vdm::DirectoryModelBuilder::withFilePathUpdater()
{
    addHandler(std::make_unique<vdm::FilePathUpdateHandler>());
    return *this;
}

//-----------------------------------------------------------------------------

vdm::DirectoryModelBuilder &vdm::DirectoryModelBuilder::withFileNameUpdater()
{
    addHandler(std::make_unique<vdm::FileNameUpdateHandler>());
    return *this;
}

//-----------------------------------------------------------------------------

std::unique_ptr<vdm::DirectoryModel> vdm::DirectoryModelBuilder::build()
{
    return std::make_unique<Impl>(std::move(m_updateHandlers));
}

//-----------------------------------------------------------------------------

void vdm::DirectoryModelBuilder::addHandler(std::unique_ptr<DirectoryModel::IUpdateHandler> &&handler)
{
    m_updateHandlers.emplace_back(std::move(handler));
}

//-----------------------------------------------------------------------------