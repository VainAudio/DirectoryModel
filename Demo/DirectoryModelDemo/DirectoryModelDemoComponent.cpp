#include "DirectoryModelDemoComponent.h"

#include "JuceTreeView/DirectoryModelDemoJuceTreeView.h"
#include "TreeView/DirectoryModelDemoTreeView.h"

//--------------------------------------------------------------------------------

DirectoryModelDemoComponent::DirectoryModelDemoComponent()
{
    m_dirModel.addUpdateHandler(m_nameUpdateHandler);
    m_dirModel.addUpdateHandler(m_fileSizeUpdateHandler);
    m_dirModel.addUpdateHandler(m_extensionUpdateHandler);
    m_dirModel.addUpdateHandler(m_pathUpdateHandler);

    m_dirModel.initialize(juce::File{ PROJECT_ROOT_DIR });
    m_dirModelSync.syncModel(m_dirModel);

    m_selectionModel.setValueTree(m_dirModel.getValueTree());

    m_selectionModel.addSelectionHandler(m_singleSelectionHandler.Mode, m_singleSelectionHandler);
    m_selectionModel.addSelectionHandler(m_individualMultiSelectionHandler.Mode, m_individualMultiSelectionHandler);
    m_selectionModel.addSelectionHandler(m_groupMultiSelectionHandler.Mode, m_groupMultiSelectionHandler);
    m_selectionModel.addSelectionHandler(m_pathSelectionHandler.Mode, m_pathSelectionHandler);

    m_selectionModel.setSelectionMode(vdm::SingleSelectionHandler::Mode);

    addAndMakeVisible(m_showRootItemToggle);
    m_showRootItemToggle.setButtonText("show root item");
    m_showRootItemToggle.onStateChange = [this]()
    {
        const bool b{ m_showRootItemToggle.getToggleState() };

        if (const auto p = dynamic_cast<DirectoryModelDemoJuceTreeView *>(m_treeView.get()))
            p->setRootItemVisible(b);
        if (const auto p = dynamic_cast<DirectoryModelDemoTreeView *>(m_treeView.get()))
        {
            p->setRootItemVisible(b);
            p->setRootItemSelectable(b);
        }
    };

    addAndMakeVisible(m_pathSelectionToggle);
    m_pathSelectionToggle.setButtonText("path selection");
    m_pathSelectionToggle.onStateChange = [this]()
    {
        m_selectionModel.setSelectionMode(m_pathSelectionToggle.getToggleState() ? vdm::PathSelectionHandler::Mode
                                                                                 : vdm::SingleSelectionHandler::Mode);
    };

    getLookAndFeel().setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::black);

    m_selectionViewer.setSelectionModelValueTree(m_dirModel.getValueTree());
    addAndMakeVisible(m_selectionViewer);

    m_treeViewTypeCbx.addItem("juce treeview", 1);
    m_treeViewTypeCbx.addItem("custom tree view", 2);
    m_treeViewTypeCbx.onChange = [this]()
    {
        switch (m_treeViewTypeCbx.getSelectedId())
        {
        case 1:
        {
            auto ptr = std::make_unique<DirectoryModelDemoJuceTreeView>();
            ptr->setValueTree(m_dirModel.getValueTree());
            m_treeView = std::move(ptr);
            break;
        }
        case 2:
        {
            auto ptr = std::make_unique<DirectoryModelDemoTreeView>();
            ptr->setValueTree(m_dirModel.getValueTree());
            ptr->setSelectionValueTree(m_dirModel.getValueTree());
            m_treeView = std::move(ptr);
            break;
        }
        default:
            jassertfalse;
        }

        addAndMakeVisible(*m_treeView);
        m_showRootItemToggle.onStateChange();
        resized();
    };
    m_treeViewTypeCbx.setSelectedId(1);
    addAndMakeVisible(m_treeViewTypeCbx);
    m_treeViewTypeCbx.setWantsKeyboardFocus(false);
}

//--------------------------------------------------------------------------------

DirectoryModelDemoComponent::~DirectoryModelDemoComponent() = default;

//--------------------------------------------------------------------------------

void DirectoryModelDemoComponent::resized()
{
    auto b = getLocalBounds();

    auto header{ b.removeFromTop(50) };
    header.reduce(7, 7);
    m_treeViewTypeCbx.setBounds(header.removeFromLeft(140));
    header.removeFromLeft(7);
    m_pathSelectionToggle.setBounds(header.removeFromLeft(100));
    m_showRootItemToggle.setBounds(header);

    m_selectionViewer.setBounds(b.removeFromLeft(250));
    if (m_treeView)
        m_treeView->setBounds(b);
}

//--------------------------------------------------------------------------------

void DirectoryModelDemoComponent::modifierKeysChanged(const juce::ModifierKeys &modifiers)
{
    if (m_pathSelectionToggle.getToggleState())
        return;

    if (modifiers.isCtrlDown())
        m_selectionModel.setSelectionMode(vdm::IndividualMultiSelectionHandler::Mode);
    else if (modifiers.isShiftDown())
        m_selectionModel.setSelectionMode(vdm::GroupMultiSelectionHandler::Mode);
    else
        m_selectionModel.setSelectionMode(vdm::SingleSelectionHandler::Mode);
}

//--------------------------------------------------------------------------------
