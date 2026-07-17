#include "vdm_SelectionModelListener.h"
#include <vdm_utility/vdm_utility.h>

//-----------------------------------------------------------------------------

class vdm::SelectionModelListener::Impl : public juce::ValueTree::Listener
{
public:
    Impl(SelectionModelListener &l)
        : listener(l)
    {
    }

    ~Impl() override
    {
        if (tree.isValid())
            tree.removeListener(this);
    }

    void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
                                  const juce::Identifier &property) override
    {
        if (property == SelectionModel::IsSelectedKey)
        {
            listener.onTreeSelectionChanged(treeWhosePropertyHasChanged);
        }
    }

    void valueTreeChildRemoved(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenRemoved,
                               int indexFromWhichChildWasRemoved) override
    {
        juce::ignoreUnused(parentTree, indexFromWhichChildWasRemoved);

        Utility::forEach(childWhichHasBeenRemoved,
                         [this](juce::ValueTree t)
                         {
                             if (SelectionModel::IsSelected(t))
                             {
                                 t.setProperty(SelectionModel::IsSelectedKey, false, nullptr);
                                 listener.onTreeSelectionChanged(t);
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

        Utility::forEach(tree, fn);
    }

    SelectionModelListener &listener;
    juce::ValueTree tree;
};

//-----------------------------------------------------------------------------

vdm::SelectionModelListener::SelectionModelListener()
    : m_p(std::make_unique<Impl>(*this))
{
}

//-----------------------------------------------------------------------------

vdm::SelectionModelListener::~SelectionModelListener() = default;

//-----------------------------------------------------------------------------

void vdm::SelectionModelListener::setSelectionModel(SelectionModel &model)
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
