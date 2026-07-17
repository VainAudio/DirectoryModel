#include "SelectionViewer.h"

//--------------------------------------------------------------------------------

SelectionViewer::~SelectionViewer() = default;

//--------------------------------------------------------------------------------

void SelectionViewer::onTreeSelectionChanged(juce::ValueTree tree)
{
    if (vdm::SelectionModel::IsSelected(tree))
        m_selectedTrees.insert(m_selectedTrees.begin(), tree);
    else
    {
        auto it = std::ranges::find_if(m_selectedTrees, [tree](auto t) { return t == tree; });
        if (it != m_selectedTrees.end())
            m_selectedTrees.erase(it);
    }

    repaint();
}

//--------------------------------------------------------------------------------

void SelectionViewer::paint(juce::Graphics &g)
{
    auto b{ getLocalBounds() };

    b.reduce(2, 2);
    g.setColour(juce::Colours::black);
    g.drawLine(juce::Line{ b.getTopRight(), b.getBottomRight() }.toFloat(), 1.0f);
    g.drawText("selected trees:", b.removeFromTop(30), juce::Justification::centred);
    b.removeFromRight(7);

    for (const auto &tree : m_selectedTrees)
    {
        auto text = juce::File{ tree.getProperty(vdm::FilePathUpdateHandler::Key).toString() }.getFileName();

        b.removeFromTop(2);
        auto textBounds{ b.removeFromTop(20) };
        g.setColour(findColour(juce::TreeView::selectedItemBackgroundColourId));
        g.fillRoundedRectangle(textBounds.toFloat(), 5.0f);
        g.setColour(juce::Colours::black);
        g.drawText(text, textBounds.withTrimmedLeft(7), juce::Justification::left);
    }
}

//--------------------------------------------------------------------------------
