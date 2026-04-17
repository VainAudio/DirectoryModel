#include "MainComponent.h"
#include "DirectoryModelDemo/DirectoryModelDemoComponent.h"

//--------------------------------------------------------------------------------

MainComponent::MainComponent()
{
    setSize(1200, 800);

    getLookAndFeel().setColour(juce::TreeView::selectedItemBackgroundColourId, juce::Colours::red.withAlpha(0.25f));
    getLookAndFeel().setColour(juce::TreeView::backgroundColourId, juce::Colours::lightgrey.darker(0.1f));

    addAndMakeVisible(m_directoryModelDemoComponent);
}

//--------------------------------------------------------------------------------

MainComponent::~MainComponent() = default;

//--------------------------------------------------------------------------------

void MainComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::lightgrey);
}

//--------------------------------------------------------------------------------

void MainComponent::resized()
{
    auto b = getLocalBounds();
    m_directoryModelDemoComponent.setBounds(b);
}

//--------------------------------------------------------------------------------
