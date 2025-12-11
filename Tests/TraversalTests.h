#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vdm_directory/vdm_directory.h>

//--------------------------------------------------------------------------------

TEST(Traversal, Cursor)
{
    juce::ValueTree root{ "root" };
    auto addTree = [](const char *id, juce::ValueTree parent)
    {
        juce::ValueTree v{ id };
        parent.addChild(v, -1, nullptr);
        return v;
    };

    auto a = addTree("a", root);
    auto b = addTree("b", root);
    auto c = addTree("c", root);
    auto d = addTree("d", root);
    auto e = addTree("e", root);
    auto f = addTree("f", root);

    vdm::Cursor cursor;
    cursor.setValueTree(root);

    EXPECT_EQ(cursor.getValueTree(), root);

    EXPECT_FALSE(cursor.next());
    EXPECT_FALSE(cursor.previous());
    EXPECT_FALSE(cursor.parent());
    EXPECT_TRUE(cursor.child());

    EXPECT_EQ(cursor.getValueTree(), a);

    EXPECT_FALSE(cursor.previous());
    EXPECT_TRUE(cursor.next());

    EXPECT_EQ(cursor.getValueTree(), b);

    EXPECT_FALSE(cursor.child());

    EXPECT_TRUE(cursor.previous());

    EXPECT_EQ(cursor.getValueTree(), a);

    EXPECT_TRUE(cursor.parent());

    EXPECT_EQ(cursor.getValueTree(), root);
}

//--------------------------------------------------------------------------------
