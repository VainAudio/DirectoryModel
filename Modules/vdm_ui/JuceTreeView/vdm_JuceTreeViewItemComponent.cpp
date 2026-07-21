#include "vdm_JuceTreeViewItemComponent.h"

//--------------------------------------------------------------------------------

vdm::JuceTreeViewItemComponent::JuceTreeViewItemComponent(juce::ValueTree tree)
    : vdm::TreeViewItemBase(tree)
    , m_tree(tree)
{
    initTreeViewItemListener();
    m_tree.addListener(this);
    for (int i = 0; i < m_tree.getNumProperties(); i++)
    {
        const auto propId{ m_tree.getPropertyName(i) };
        vdm::JuceTreeViewItemComponent::valueTreePropertyChanged(m_tree, propId);
    }
}

//--------------------------------------------------------------------------------

vdm::JuceTreeViewItemComponent::~JuceTreeViewItemComponent()
{
    m_tree.removeListener(this);
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeViewItemComponent::paint(juce::Graphics &g)
{
    const auto b{ getLocalBounds() };

    g.setColour(juce::Colours::black);

    const auto text = juce::File{ m_tree.getProperty(vdm::FilePathUpdateHandler::Key).toString() }.getFileName();
    g.drawText(text, b, juce::Justification::left);

    g.drawText(m_sizeString, b.withTrimmedRight(5), juce::Justification::right);
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeViewItemComponent::mouseUp(const juce::MouseEvent &event)
{
    if (!event.mods.isLeftButtonDown())
        return;

    vdm::SelectionModel::RequestToggleSelection(m_tree);
    repaint();
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeViewItemComponent::mouseDoubleClick(const juce::MouseEvent &event)
{
    using vdm = vdm::DirectoryModel;
    if (event.mods.isLeftButtonDown() && vdm::IsDir(m_tree))
        vdm::ToggleIsDirOpen(m_tree);
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeViewItemComponent::onItemSelectedChanged(bool isSelected)
{
    m_isSelected = isSelected;
    repaint();
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeViewItemComponent::onItemIsDirOpenChanged(bool isDirOpen)
{
    juce::ignoreUnused(isDirOpen);
    repaint();
}

//--------------------------------------------------------------------------------

void vdm::JuceTreeViewItemComponent::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
                                                              const juce::Identifier &property)
{
    if (treeWhosePropertyHasChanged != m_tree)
        return;

    if (property == vdm::FileSizeUpdateHandler::Key)
    {
        constexpr std::array<const char *, 5> sizes{
            {
             "B", "kB",
             "mB", "gB",
             "tB", }
        };

        const float bytes{ m_tree.getProperty(vdm::FileSizeUpdateHandler::Key) };
        for (int i = sizes.size() - 1; i >= 0; i--)
        {
            const float value{ std::pow(1000.0f, static_cast<float>(i)) };
            if (value <= bytes)
            {
                const float fileSize{ bytes / value };
                m_sizeString = juce::String(fileSize, juce::exactlyEqual(fileSize, std::floor(fileSize)) ? 0 : 2) +
                               sizes[static_cast<std::size_t>(i)];
                break;
            }
        }

        repaint();
    }
}

//--------------------------------------------------------------------------------
