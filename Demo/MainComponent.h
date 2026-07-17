#pragma once

//--------------------------------------------------------------------------------

#include <juce_gui_basics/juce_gui_basics.h>
#include "DirectoryModelDemo/DirectoryModelDemoComponent.h"

//--------------------------------------------------------------------------------

class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics &) override;
    void resized() override;

private:
    DirectoryModelDemoComponent m_directoryModelDemoComponent;
};

//--------------------------------------------------------------------------------
