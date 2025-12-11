#include "vdm_SelectionModelListener.h"

//-----------------------------------------------------------------------------

class vdm::SelectionModelListener::Impl : public juce::ValueTree::Listener
{
public:
    Impl(SelectionModelListener &l)
        : listener(l)
    {}

    ~Impl() override
    {
        if (tree.isValid())
            tree.removeListener(this);
    }

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override
    {
        if (property == SelectionModel::IsSelectedKey)
        {
            listener.onSelectedTreeChanged(treeWhosePropertyHasChanged);
        }
    }

    template<typename Fn>
    static void traverseApply(juce::ValueTree tree, const Fn &fn)
    {
        fn(tree);
        for (auto child : tree)
            traverseApply(child, fn);
    }

    void valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override
    {
        juce::ignoreUnused(parentTree, indexFromWhichChildWasRemoved);

        traverseApply(childWhichHasBeenRemoved, [this](juce::ValueTree t)
        {
            if (SelectionModel::IsSelected(t))
            {
                t.setProperty(SelectionModel::IsSelectedKey, false, nullptr);
                listener.onSelectedTreeChanged(t);
            }
        });
    }

    void sendCallbacks()
    {
        auto fn = [this](juce::ValueTree t)
        {
            if (SelectionModel::IsSelected(t))
                valueTreePropertyChanged(t, SelectionModel::IsSelectedKey);
        };

        traverseApply(tree, fn);
    }

    SelectionModelListener &listener;
    juce::ValueTree tree;
};

//-----------------------------------------------------------------------------

vdm::SelectionModelListener::SelectionModelListener()
    : m_p(std::make_unique<Impl>(*this))
{}

//-----------------------------------------------------------------------------

vdm::SelectionModelListener::~SelectionModelListener() = default;

//-----------------------------------------------------------------------------

void vdm::SelectionModelListener::setSelectionModel(SelectionModel& model)
{
    setSelectionModelValueTree(model.getValueTree());
}

//-----------------------------------------------------------------------------

void vdm::SelectionModelListener::setSelectionModelValueTree(juce::ValueTree tree)
{
    m_p->tree = tree;
    m_p->tree.addListener(m_p.get());
    m_p->sendCallbacks();
}

//-----------------------------------------------------------------------------
