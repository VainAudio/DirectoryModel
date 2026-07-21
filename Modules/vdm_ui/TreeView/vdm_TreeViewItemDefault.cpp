#include "vdm_TreeViewItemDefault.h"

//--------------------------------------------------------------------------------

vdm::TreeViewItemDefault::TreeViewItemDefault(juce::ValueTree tree)
    : vdm::TreeViewItemBase(tree)
    , m_tree(tree)
{
    {
        m_tree.addListener(this);

        for (int i = 0; i < m_tree.getNumProperties(); i++)
        {
            const auto propId{ m_tree.getPropertyName(i) };
            vdm::TreeViewItemDefault::valueTreePropertyChanged(m_tree, propId);
        }
    }
}

//--------------------------------------------------------------------------------

vdm::TreeViewItemDefault::~TreeViewItemDefault()
{
    m_tree.removeListener(this);
}

//--------------------------------------------------------------------------------

void vdm::TreeViewItemDefault::paint(juce::Graphics &g)
{
    auto b{ getLocalBounds() };

    const auto bg = findColour(juce::TreeView::backgroundColourId);
    const auto selectedBg = findColour(juce::TreeView::selectedItemBackgroundColourId);

    if (m_isSelected)
        g.setColour(selectedBg);
    else if (isMouseOver(true))
        g.setColour(bg.interpolatedWith(selectedBg, 0.5));
    else
        g.setColour(bg);

    g.fillRoundedRectangle(b.toFloat(), 5.0f);

    g.setColour(juce::Colours::black);

    auto ab{ b.removeFromLeft(b.getHeight()).toFloat() };
    if (vdm::DirectoryModel::IsDir(m_tree))
    {
        juce::Graphics::ScopedSaveState state{ g };
        if (!vdm::DirectoryModel::IsDirOpen(m_tree))
            g.addTransform(
                juce::AffineTransform::rotation(-juce::MathConstants<float>::halfPi, ab.getCentreX(), ab.getCentreY()));

        constexpr float delta{ 7.0f };
        ab.reduce(delta, delta);
        juce::Path p;
        p.startNewSubPath(ab.getTopLeft());
        p.lineTo(ab.getTopRight());
        ab.removeFromLeft(ab.getWidth() / 2.0f);
        p.lineTo(ab.getBottomLeft());
        p.closeSubPath();
        g.fillPath(p);
    }

    auto text = juce::File{ m_tree.getProperty(vdm::FilePathUpdateHandler::Key).toString() }.getFileName();
    g.drawText(text, b, juce::Justification::left);

    g.drawText(m_sizeString, b.withTrimmedRight(5), juce::Justification::right);
}

//--------------------------------------------------------------------------------

void vdm::TreeViewItemDefault::mouseUp(const juce::MouseEvent &event)
{
    if (!event.mods.isLeftButtonDown())
        return;

    if (vdm::DirectoryModel::IsDir(m_tree) &&
        getLocalBounds().removeFromLeft(getHeight()).toFloat().contains(event.position))
        vdm::DirectoryModel::ToggleIsDirOpen(m_tree);
    else
        vdm::SelectionModel::RequestSelectTree(m_tree);
}

//--------------------------------------------------------------------------------

void vdm::TreeViewItemDefault::mouseEnter(const juce::MouseEvent &event)
{
    juce::ignoreUnused(event);
    repaint();
}

//--------------------------------------------------------------------------------

void vdm::TreeViewItemDefault::mouseExit(const juce::MouseEvent &event)
{
    juce::ignoreUnused(event);
    repaint();
}

//--------------------------------------------------------------------------------

void vdm::TreeViewItemDefault::mouseDoubleClick(const juce::MouseEvent &event)
{
    using vdm = vdm::DirectoryModel;
    if (event.mods.isLeftButtonDown() && vdm::IsDir(m_tree))
        vdm::ToggleIsDirOpen(m_tree);
}

//--------------------------------------------------------------------------------

void vdm::TreeViewItemDefault::onItemSelectedChanged(bool isSelected)
{
    m_isSelected = isSelected;
    repaint();
}

//--------------------------------------------------------------------------------

void vdm::TreeViewItemDefault::onItemIsDirOpenChanged(bool isDirOpen)
{
    juce::ignoreUnused(isDirOpen);
    repaint();
}

//--------------------------------------------------------------------------------

void vdm::TreeViewItemDefault::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
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
