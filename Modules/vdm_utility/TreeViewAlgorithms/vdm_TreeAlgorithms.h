#pragma once

//-----------------------------------------------------------------------------

#include <vdm/vdm.h>
#include <juce_data_structures/juce_data_structures.h>

//-----------------------------------------------------------------------------

BEGIN_VDM_NAMESPACE

//--------------------------------------------------------------------------------

namespace Utility
{
    /**
     *
     * @tparam Fn a callable that takes a single value tree as an argument and returns bool
     * @param root the root of the value tree to begin searching from
     * @param pred test function
     * @return First value tree that matches the predicate
     */
    template <typename Fn> juce::ValueTree findFirst(juce::ValueTree root, Fn pred)
    {
        if (pred(root))
            return root;

        for (auto child : root)
        {
            if (const auto t = findFirst(child, pred); t.isValid())
                return t;
        }

        return {};
    }

    /**
     *
     * @tparam Fn a callable that takes a single value tree as an argument and returns void
     * @param tree root tree to begin from
     * @param fn function to be applied to everything
     */
    template <typename Fn> static void forEach(juce::ValueTree tree, const Fn &fn)
    {
        fn(tree);
        for (auto child : tree)
            forEach(child, fn);
    }

}

//--------------------------------------------------------------------------------

END_VDM_NAMESPACE

//-----------------------------------------------------------------------------
