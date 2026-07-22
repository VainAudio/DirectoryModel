#pragma once

//--------------------------------------------------------------------------------

#include <juce_gui_basics/juce_gui_basics.h>
#include <vdm_directory/vdm_directory.h>

#include "SelectionViewer.h"

//--------------------------------------------------------------------------------

class DirectoryModelDemoComponent : public juce::Component
{
public:
    DirectoryModelDemoComponent();
    ~DirectoryModelDemoComponent() override;

    void resized() override;
    void modifierKeysChanged(const juce::ModifierKeys &modifiers) override;

private:
    const std::unique_ptr<vdm::DirectoryModel> m_dirModel{ vdm::DirectoryModelBuilder{}
                                                               .withFileNameUpdater()
                                                               .withFileSizeUpdater()
                                                               .withFileExtensionUpdater()
                                                               .withFilePathUpdater()
                                                               .build() };

    vdm::DirectoryModelSync m_dirModelSync;
    vdm::SelectionModel m_selectionModel;

    juce::ToggleButton m_showRootItemToggle;
    juce::ToggleButton m_pathSelectionToggle;

    vdm::SingleSelectionHandler m_singleSelectionHandler;
    vdm::IndividualMultiSelectionHandler m_individualMultiSelectionHandler;
    vdm::GroupMultiSelectionHandler m_groupMultiSelectionHandler;
    vdm::PathSelectionHandler m_pathSelectionHandler;

    SelectionViewer m_selectionViewer;

    juce::ComboBox m_treeViewTypeCbx;
    std::unique_ptr<juce::Component> m_treeView;
};

//--------------------------------------------------------------------------------
