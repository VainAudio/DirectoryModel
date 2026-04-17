#pragma once

//--------------------------------------------------------------------------------

#include <juce_gui_basics/juce_gui_basics.h>
#include <vdm_directory/vdm_directory.h>

//--------------------------------------------------------------------------------

class SelectionViewer
    : public juce::Component
    , public vdm::SelectionModelListener
{
public:
    ~SelectionViewer() override;

    void paint(juce::Graphics &g) override;

private:
    void onTreeSelectionChanged(juce::ValueTree tree) override;

    std::vector<juce::ValueTree> m_selectedTrees;
};

//--------------------------------------------------------------------------------
