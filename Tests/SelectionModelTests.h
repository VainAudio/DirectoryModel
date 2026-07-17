#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vdm_directory/vdm_directory.h>

//--------------------------------------------------------------------------------

TEST(SelectionModel, SingleSelection)
{
    juce::ValueTree root{ "root" };
    juce::ValueTree childA{ "childA" };
    juce::ValueTree childB{ "childB" };
    juce::ValueTree childC{ "childC" };

    root.addChild(childA, -1, nullptr);
    root.addChild(childB, -1, nullptr);
    root.addChild(childC, -1, nullptr);

    vdm::SelectionModel selectionModel;
    selectionModel.setValueTree(root);
    vdm::SingleSelectionHandler selectionHandler;
    selectionModel.addSelectionHandler(selectionHandler.Mode, selectionHandler);

    vdm::SelectionModel::SetSelectionMode(root, selectionHandler.Mode);

    vdm::SelectionModel::RequestSelectTree(childA);

    EXPECT_TRUE(vdm::SelectionModel::IsSelected(childA));

    vdm::SelectionModel::RequestSelectTree(childB);

    EXPECT_TRUE(vdm::SelectionModel::IsSelected(childB));
    EXPECT_FALSE(vdm::SelectionModel::IsSelected(childA));
}

//--------------------------------------------------------------------------------

struct PathSelectionTestHelper
{
    explicit PathSelectionTestHelper(juce::ValueTree parent)
        : A(parent.getType() + "_A")
        , B(parent.getType() + "_B")
        , C(parent.getType() + "_C")
    {
        parent.addChild(A, -1, nullptr);
        parent.addChild(B, -1, nullptr);
        parent.addChild(C, -1, nullptr);
    }

    juce::ValueTree A;
    juce::ValueTree B;
    juce::ValueTree C;
};

TEST(SelectionModel, PathSelection)
{
    juce::ValueTree root{ "root" };
    PathSelectionTestHelper rootChildren(root);
    PathSelectionTestHelper aChildren(rootChildren.A);
    PathSelectionTestHelper bChildren(rootChildren.B);
    PathSelectionTestHelper cChildren(rootChildren.C);

    vdm::SelectionModel selectionModel;
    selectionModel.setValueTree(root);
    vdm::PathSelectionHandler selectionHandler;
    selectionModel.addSelectionHandler(selectionHandler.Mode, selectionHandler);

    vdm::SelectionModel::SetSelectionMode(root, selectionHandler.Mode);

    vdm::SelectionModel::RequestSelectTree(rootChildren.A);

    EXPECT_EQ(vdm::SelectionModel::IsSelected(root), true);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(rootChildren.A), true);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(aChildren.A), false);

    vdm::SelectionModel::RequestSelectTree(bChildren.C);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(root), true);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(rootChildren.B), true);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(bChildren.C), true);

    EXPECT_EQ(vdm::SelectionModel::IsSelected(rootChildren.A), false);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(aChildren.A), false);
}

//--------------------------------------------------------------------------------

TEST(SelectionModel, GroupMultiSelection)
{
    juce::ValueTree root{ "root" };
    juce::ValueTree childA{ "childA" };
    juce::ValueTree childB{ "childB" };
    juce::ValueTree childC{ "childC" };
    juce::ValueTree childD{ "childD" };
    juce::ValueTree childE{ "childE" };
    juce::ValueTree childF{ "childF" };

    root.addChild(childA, -1, nullptr);
    root.addChild(childB, -1, nullptr);
    root.addChild(childC, -1, nullptr);
    root.addChild(childD, -1, nullptr);
    root.addChild(childE, -1, nullptr);
    root.addChild(childF, -1, nullptr);

    vdm::SelectionModel selectionModel;
    selectionModel.setValueTree(root);
    vdm::GroupMultiSelectionHandler selectionHandler;
    selectionModel.addSelectionHandler(selectionHandler.Mode, selectionHandler);

    vdm::SelectionModel::SetSelectionMode(root, selectionHandler.Mode);

    vdm::SelectionModel::RequestSelectTree(childA);
    EXPECT_TRUE(vdm::SelectionModel::IsSelected(childA));

    vdm::SelectionModel::RequestSelectTree(childE);

    EXPECT_TRUE(vdm::SelectionModel::IsSelected(childA));
    EXPECT_TRUE(vdm::SelectionModel::IsSelected(childB));
    EXPECT_TRUE(vdm::SelectionModel::IsSelected(childC));
    EXPECT_TRUE(vdm::SelectionModel::IsSelected(childD));
    EXPECT_TRUE(vdm::SelectionModel::IsSelected(childE));

    EXPECT_FALSE(vdm::SelectionModel::IsSelected(childF));

    vdm::SelectionModel::RequestSelectTree(childC);

    EXPECT_TRUE(vdm::SelectionModel::IsSelected(childA));
    EXPECT_TRUE(vdm::SelectionModel::IsSelected(childB));
    EXPECT_TRUE(vdm::SelectionModel::IsSelected(childC));

    EXPECT_FALSE(vdm::SelectionModel::IsSelected(childD));
    EXPECT_FALSE(vdm::SelectionModel::IsSelected(childE));
    EXPECT_FALSE(vdm::SelectionModel::IsSelected(childF));
}

//--------------------------------------------------------------------------------

TEST(SelectionModel, IndividualMultiSelection)
{
    juce::ValueTree root{ "root" };
    juce::ValueTree childA{ "childA" };
    juce::ValueTree childB{ "childB" };
    juce::ValueTree childC{ "childC" };
    juce::ValueTree childD{ "childD" };
    juce::ValueTree childE{ "childE" };
    juce::ValueTree childF{ "childF" };

    root.addChild(childA, -1, nullptr);
    root.addChild(childB, -1, nullptr);
    root.addChild(childC, -1, nullptr);
    root.addChild(childD, -1, nullptr);
    root.addChild(childE, -1, nullptr);
    root.addChild(childF, -1, nullptr);

    vdm::SelectionModel selectionModel;
    selectionModel.setValueTree(root);
    vdm::IndividualMultiSelectionHandler selectionHandler;
    selectionModel.addSelectionHandler(selectionHandler.Mode, selectionHandler);

    vdm::SelectionModel::SetSelectionMode(root, selectionHandler.Mode);

    vdm::SelectionModel::RequestSelectTree(childA);
    vdm::SelectionModel::RequestSelectTree(childC);
    vdm::SelectionModel::RequestSelectTree(childE);

    EXPECT_EQ(vdm::SelectionModel::IsSelected(childA), true);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(childB), false);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(childC), true);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(childD), false);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(childE), true);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(childF), false);

    vdm::SelectionModel::RequestSelectTree(childA);
    vdm::SelectionModel::RequestSelectTree(childB);
    vdm::SelectionModel::RequestSelectTree(childF);

    EXPECT_EQ(vdm::SelectionModel::IsSelected(childA), false);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(childB), true);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(childC), true);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(childD), false);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(childE), true);
    EXPECT_EQ(vdm::SelectionModel::IsSelected(childF), true);
}

//--------------------------------------------------------------------------------
using ::testing::_;

class MockSelectionModelListener : public vdm::SelectionModelListener
{
public:
    MOCK_METHOD(void, onTreeSelectionChanged, (juce::ValueTree), (override));
};

//--------------------------------------------------------------------------------

TEST(SelectionModel, SelectionListenerInit)
{
    juce::ValueTree root{ "root" };
    juce::ValueTree childA{ "childA" };
    juce::ValueTree childB{ "childB" };
    juce::ValueTree childC{ "childC" };

    root.addChild(childA, -1, nullptr);
    root.addChild(childB, -1, nullptr);
    root.addChild(childC, -1, nullptr);

    vdm::SelectionModel selectionModel;
    selectionModel.setValueTree(root);
    vdm::IndividualMultiSelectionHandler selectionHandler;
    selectionModel.addSelectionHandler(selectionHandler.Mode, selectionHandler);

    vdm::SelectionModel::SetSelectionMode(root, selectionHandler.Mode);

    vdm::SelectionModel::RequestSelectTree(childA);
    vdm::SelectionModel::RequestSelectTree(childB);

    MockSelectionModelListener listener;

    EXPECT_CALL(listener, onTreeSelectionChanged(_)).Times(2);
    listener.setSelectionModelValueTree(root);
}

//--------------------------------------------------------------------------------

TEST(SelectionModel, SelectionListener)
{
    juce::ValueTree root{ "root" };
    juce::ValueTree childA{ "childA" };
    juce::ValueTree childB{ "childB" };
    juce::ValueTree childC{ "childC" };

    root.addChild(childA, -1, nullptr);
    root.addChild(childB, -1, nullptr);
    root.addChild(childC, -1, nullptr);

    vdm::SelectionModel selectionModel;
    selectionModel.setValueTree(root);
    vdm::SingleSelectionHandler selectionHandler;
    selectionModel.addSelectionHandler(selectionHandler.Mode, selectionHandler);

    vdm::SelectionModel::SetSelectionMode(root, selectionHandler.Mode);

    MockSelectionModelListener listener;

    EXPECT_CALL(listener, onTreeSelectionChanged(_)).Times(1);
    listener.setSelectionModelValueTree(root);

    vdm::SelectionModel::RequestSelectTree(childA);
}

//--------------------------------------------------------------------------------

TEST(DirectoryCursor, Basic)
{
    juce::ValueTree root{ "root" };
    root.setProperty(vdm::DirectoryModel::Keys::FileType, static_cast<int>(vdm::DirectoryModel::Type::Dir), nullptr);

    auto addTree = [](const char *id, juce::ValueTree parent)
    {
        juce::ValueTree v{ parent.getType().toString() + "_" + id };
        v.setProperty(vdm::DirectoryModel::Keys::FileType, static_cast<int>(vdm::DirectoryModel::Type::Dir), nullptr);
        parent.addChild(v, -1, nullptr);
        return v;
    };

    auto a = addTree("a", root);
    auto b = addTree("b", root);

    auto a_a = addTree("a", a);
    auto a_b = addTree("b", a);
    auto a_c = addTree("c", a);

    auto b_a = addTree("a", b);
    auto b_b = addTree("b", b);
    auto b_c = addTree("c", b);

    vdm::TreeViewCursor cursor;
    cursor.setValueTree(root);

    auto isFocused = [&cursor](juce::ValueTree tree) { return cursor.getValueTree() == tree; };

    EXPECT_TRUE(isFocused(root));

    cursor.keyUp();

    EXPECT_TRUE(isFocused(root));
    EXPECT_FALSE(vdm::DirectoryModel::IsDirOpen(root));

    cursor.keyRight();
    EXPECT_TRUE(vdm::DirectoryModel::IsDirOpen(root));

    cursor.keyRight();
    EXPECT_FALSE(isFocused(root));
    EXPECT_TRUE(isFocused(a));
    EXPECT_FALSE(vdm::DirectoryModel::IsDirOpen(a));

    cursor.keyRight();
    EXPECT_TRUE(vdm::DirectoryModel::IsDirOpen(a));

    cursor.keyDown();
    EXPECT_FALSE(isFocused(a));
    EXPECT_TRUE(isFocused(a_a));

    cursor.keyUp();
    EXPECT_FALSE(isFocused(a_a));
    EXPECT_TRUE(isFocused(a));
    vdm::DirectoryModel::SetDirOpen(a, false);

    cursor.keyDown();
    cursor.keyLeft();
    EXPECT_TRUE(isFocused(root));

    cursor.keyDown();
    EXPECT_TRUE(isFocused(a));
    vdm::DirectoryModel::SetDirOpen(a, true);

    cursor.keyDown();
    EXPECT_TRUE(isFocused(a_a));

    cursor.keyDown();
    EXPECT_TRUE(isFocused(a_b));
}

//--------------------------------------------------------------------------------

TEST(DirectorySelection, Basic)
{
    juce::ValueTree root{ "root" };
    root.setProperty(vdm::DirectoryModel::Keys::FileType, static_cast<int>(vdm::DirectoryModel::Type::Dir), nullptr);

    auto addTree = [](const char *id, juce::ValueTree parent)
    {
        juce::ValueTree v{ parent.getType().toString() + "_" + id };
        v.setProperty(vdm::DirectoryModel::Keys::FileType, static_cast<int>(vdm::DirectoryModel::Type::Dir), nullptr);
        parent.addChild(v, -1, nullptr);
        return v;
    };

    auto a = addTree("a", root);
    auto b = addTree("b", root);

    auto a_a = addTree("a", a);
    auto a_b = addTree("b", a);
    auto a_c = addTree("c", a);

    auto b_a = addTree("a", b);
    auto b_b = addTree("b", b);
    auto b_c = addTree("c", b);

    vdm::SelectionModel selectionModel{ root };
    vdm::SingleSelectionHandler selectionHandler;
    selectionModel.addSelectionHandler(vdm::SingleSelectionHandler::Mode, selectionHandler);
    selectionModel.setSelectionMode(vdm::SingleSelectionHandler::Mode);

    auto isSelected = [](juce::ValueTree tree) { return vdm::SelectionModel::IsSelected(tree); };

    vdm::TreeViewSelector selector;
    selector.setValueTree(root);

    EXPECT_TRUE(isSelected(root));

    selector.keyUp();

    EXPECT_TRUE(isSelected(root));
    EXPECT_FALSE(vdm::DirectoryModel::IsDirOpen(root));

    selector.keyRight();
    EXPECT_TRUE(vdm::DirectoryModel::IsDirOpen(root));

    selector.keyRight();
    EXPECT_FALSE(isSelected(root));
    EXPECT_TRUE(isSelected(a));
    EXPECT_FALSE(vdm::DirectoryModel::IsDirOpen(a));

    selector.keyRight();
    EXPECT_TRUE(vdm::DirectoryModel::IsDirOpen(a));

    selector.keyDown();
    EXPECT_FALSE(isSelected(a));
    EXPECT_TRUE(isSelected(a_a));

    selector.keyUp();
    EXPECT_FALSE(isSelected(a_a));
    EXPECT_TRUE(isSelected(a));
    vdm::DirectoryModel::SetDirOpen(a, false);

    selector.keyDown();
    selector.keyLeft();
    EXPECT_TRUE(isSelected(root));

    selector.keyDown();
    EXPECT_TRUE(isSelected(a));
    vdm::DirectoryModel::SetDirOpen(a, true);

    selector.keyDown();
    EXPECT_TRUE(isSelected(a_a));

    selector.keyDown();
    EXPECT_TRUE(isSelected(a_b));
}

//--------------------------------------------------------------------------------
