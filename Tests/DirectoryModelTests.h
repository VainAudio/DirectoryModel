#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vdm_directory/vdm_directory.h>
#include "TestHelpers.h"
#include "vdm_directory/DirectoryModel/vdm_DirectoryModel.h"

//--------------------------------------------------------------------------------

static juce::StringArray GetChildNames(juce::ValueTree tree)
{
    juce::StringArray names;
    for (int i = 0; i < tree.getNumChildren(); ++i)
    {
        names.add(tree.getChild(i).getType().toString());
    }
    return names;
}

//--------------------------------------------------------------------------------

TEST(DirectoryTree, CreateTree)
{
    DirTestHelper::InitDirTest();

    DirTestHelper dir;
    auto child = dir.createFile("child.txt");

    DirTestHelper dir2 = dir.createDir("asdf");
    auto dirChild = dir2.createFile("dir_child.txt");

    vdm::DirectoryTree tree{ dir.file() };

    EXPECT_FALSE(tree.hasValueTreeAt(child));
    auto childTree = tree.getValueTreeAt(child);
    EXPECT_TRUE(tree.hasValueTreeAt(child));
    EXPECT_TRUE(childTree.isValid());
    EXPECT_EQ(tree.getRootValueTree().getNumChildren(), 1);
    EXPECT_EQ(childTree.getType().toString(), "child.txt");

    auto dirChildTree = tree.getValueTreeAt(dirChild);
    EXPECT_EQ(tree.getRootValueTree().getNumChildren(), 2);
    EXPECT_EQ(dirChildTree.getType().toString(), "dir_child.txt");
    EXPECT_EQ(dirChildTree.getParent().getType().toString(), "asdf");

    tree.removeValueTreeAt(dirChild.getParentDirectory());
    EXPECT_EQ(tree.getRootValueTree().getNumChildren(), 1);
}

//--------------------------------------------------------------------------------

TEST(DirectoryModel, CreateInitialTree)
{
    const MessageManagerHelper mm; // this test keeps the message thread locked, no updates to the file model

    DirTestHelper::InitDirTest();

    DirTestHelper dir;
    dir.createFile("test1.txt");
    dir.createFile("test2.txt");
    dir.createFile("test3.txt");

    DirTestHelper dir2 = dir.createDir("asdf");
    dir2.createFile("dir_test1.txt");
    dir2.createFile("dir_test2.txt");

    vdm::DirectoryModel model;
    model.initialize(dir.file());

    EXPECT_EQ(model.getValueTree().getNumChildren(), 4);

    const auto names{ GetChildNames(model.getValueTree()) };
    EXPECT_TRUE(names.contains("test1.txt"));
    EXPECT_TRUE(names.contains("test2.txt"));
    EXPECT_TRUE(names.contains("test3.txt"));
    EXPECT_TRUE(names.contains("asdf"));

    EXPECT_EQ(model.getValueTree().getChildWithName("asdf").getNumChildren(), 2);
    const auto names2{ GetChildNames(model.getValueTree().getChildWithName("asdf")) };

    EXPECT_TRUE(names2.contains("dir_test1.txt"));
    EXPECT_TRUE(names2.contains("dir_test2.txt"));
}

//--------------------------------------------------------------------------------

TEST(DirectoryModel, AddToTree)
{
    const MessageManagerHelper mm;

    DirTestHelper::InitDirTest();

    DirTestHelper dir;

    vdm::DirectoryModel model;
    model.initialize(dir.file());
    vdm::DirectoryModelSync sync;
    sync.syncModel(model);

    EXPECT_EQ(model.getValueTree().getNumChildren(), 0);

    dir.createFile("test1.txt");
    dir.createFile("test2.txt");
    dir.createFile("test3.txt");

    DirTestHelper dir2 = dir.createDir("asdf");

    mm.runMessageThread();

    dir2.createFile("dir_test1.txt");
    dir2.createFile("dir_test2.txt");

    mm.runMessageThread();

    EXPECT_EQ(model.getValueTree().getNumChildren(), 4);

    const auto names{ GetChildNames(model.getValueTree()) };
    EXPECT_TRUE(names.contains("test1.txt"));
    EXPECT_TRUE(names.contains("test2.txt"));
    EXPECT_TRUE(names.contains("test3.txt"));
    EXPECT_TRUE(names.contains("asdf"));

    EXPECT_EQ(model.getValueTree().getChildWithName("asdf").getNumChildren(), 2);
    const auto names2{ GetChildNames(model.getValueTree().getChildWithName("asdf")) };

    EXPECT_TRUE(names2.contains("dir_test1.txt"));
    EXPECT_TRUE(names2.contains("dir_test2.txt"));
}

//--------------------------------------------------------------------------------

TEST(DirectoryModel, RemoveFromTree)
{
    const MessageManagerHelper mm;

    DirTestHelper::InitDirTest();

    DirTestHelper dir;
    dir.createFile("test1.txt");
    dir.createFile("test2.txt");
    dir.createFile("test3.txt");

    DirTestHelper dir2 = dir.createDir("asdf");
    dir2.createFile("dir_test1.txt");
    dir2.createFile("dir_test2.txt");

    mm.runMessageThread();

    vdm::DirectoryModel model;
    model.initialize(dir.file());
    vdm::DirectoryModelSync sync;
    sync.syncModel(model);

    dir2.deleteFile("dir_test2.txt");
    mm.runMessageThread();
    EXPECT_EQ(model.getValueTree().getChildWithName("asdf").getNumChildren(), 1);

    dir2.deleteFile("dir_test1.txt");
    mm.runMessageThread();
    EXPECT_EQ(model.getValueTree().getChildWithName("asdf").getNumChildren(), 0);

    dir.deleteFile("test1.txt");
    mm.runMessageThread();
    EXPECT_EQ(model.getValueTree().getNumChildren(), 3);

    dir.deleteDir("asdf");
    mm.runMessageThread();
    EXPECT_EQ(model.getValueTree().getNumChildren(), 2);
}

//--------------------------------------------------------------------------------

using ::testing::_;

class MockAddHandler : public vdm::DirectoryModel::IUpdateHandler
{
public:
    ~MockAddHandler() override = default;

    MOCK_METHOD(void, onAdd, (juce::ValueTree, juce::File), (override));
    // MOCK_METHOD(void, onModify, (juce::ValueTree, juce::File), (override));
    void onModify(juce::ValueTree, juce::File) override {}
};

class MockModifyHandler : public vdm::DirectoryModel::IUpdateHandler
{
public:
    ~MockModifyHandler() override = default;

    void onAdd(juce::ValueTree, juce::File) override {}
    MOCK_METHOD(void, onModify, (juce::ValueTree, juce::File), (override));
};

//--------------------------------------------------------------------------------

TEST(DirectoryModel, OnInitUpdateHandler)
{
    const MessageManagerHelper mm;

    DirTestHelper::InitDirTest();

    DirTestHelper dir;
    dir.createFile("test1.txt");
    dir.createFile("test2.txt");
    dir.createFile("test3.txt");

    vdm::DirectoryModel model;

    MockAddHandler mock;
    EXPECT_CALL(mock, onAdd(_, _)).Times(4);

    model.addUpdateHandler(mock);

    model.initialize(dir.file());
}

//--------------------------------------------------------------------------------

TEST(DirectoryModel, OnMessageUpdateHandler)
{
    const MessageManagerHelper mm;

    DirTestHelper::InitDirTest();

    DirTestHelper dir;

    vdm::DirectoryModel model;

    MockAddHandler mock;
    EXPECT_CALL(mock, onAdd(_, _)).Times(4);

    dir.createFile("test1.txt");
    dir.createFile("test2.txt");
    dir.createFile("test3.txt");

    mm.runMessageThread();

    model.addUpdateHandler(mock);

    model.initialize(dir.file());
}

//--------------------------------------------------------------------------------

TEST(DirectoryModel, OnModifyUpdateHandler)
{
    const MessageManagerHelper mm;

    DirTestHelper::InitDirTest();

    DirTestHelper dir;
    dir.createFile("a_test1.txt");
    dir.createFile("a_test2.txt");
    dir.createFile("a_test3.txt");

    vdm::DirectoryModel model;

    MockModifyHandler mock;
    EXPECT_CALL(mock, onModify(_, _)).Times(testing::AnyNumber());
    model.addUpdateHandler(mock);

    model.initialize(dir.file());
    vdm::DirectoryModelSync sync;
    sync.syncModel(model);

    {
        juce::File f{ dir.file().getChildFile("a_test1.txt") };
        juce::FileOutputStream os{ f };
        os.writeString("asdf");
        os.flush();
    }

    mm.runMessageThread();
}

//--------------------------------------------------------------------------------

TEST(DirectoryModel, NameUpdateHandler)
{
    const MessageManagerHelper mm;

    DirTestHelper::InitDirTest();

    DirTestHelper dir;
    dir.createFile("test1.txt");
    dir.createFile("test2.txt");
    dir.createFile("test3.txt");

    vdm::DirectoryModel model;

    vdm::NameUpdateHandler nameUpdateHandler;
    model.addUpdateHandler(nameUpdateHandler);

    model.initialize(dir.file());

    mm.runMessageThread();

    auto getName = [&model](const juce::String &file)
    {
        return model.getValueTree().getChildWithName(file).getProperty(vdm::NameUpdateHandler::Key).toString().toStdString();
    };

    EXPECT_EQ(getName("test1.txt"), "test1");
    EXPECT_EQ(getName("test2.txt"), "test2");
    EXPECT_EQ(getName("test3.txt"), "test3");
}

//--------------------------------------------------------------------------------

TEST(DirectoryModel, ExtensionUpdateHandler)
{
    const MessageManagerHelper mm;

    DirTestHelper::InitDirTest();

    DirTestHelper dir;
    dir.createFile("test1.txt1");
    dir.createFile("test2.txt2");
    dir.createFile("test3.txt3");

    vdm::DirectoryModel model;

    vdm::ExtensionUpdateHandler extensionUpdateHandler;
    model.addUpdateHandler(extensionUpdateHandler);

    model.initialize(dir.file());

    mm.runMessageThread();

    auto getExt = [&model](const juce::String &file)
    {
        return model.getValueTree().getChildWithName(file).getProperty(vdm::ExtensionUpdateHandler::Key).toString().toStdString();
    };

    EXPECT_EQ(getExt("test1.txt1"), "txt1");
    EXPECT_EQ(getExt("test2.txt2"), "txt2");
    EXPECT_EQ(getExt("test3.txt3"), "txt3");
}

//--------------------------------------------------------------------------------

TEST(DirectoryModel, PathUpdateHandler)
{
    const MessageManagerHelper mm;

    DirTestHelper::InitDirTest();

    DirTestHelper dir;
    dir.createFile("test1.txt1");
    dir.createFile("test2.txt2");
    dir.createFile("test3.txt3");

    vdm::DirectoryModel model;

    vdm::PathUpdateHandler pathUpdateHandler;
    model.addUpdateHandler(pathUpdateHandler);

    model.initialize(dir.file());

    mm.runMessageThread();

    auto getExt = [&model](const juce::String &file)
    {
        return model.getValueTree().getChildWithName(file).getProperty(vdm::PathUpdateHandler::Key).toString().toStdString();
    };

    auto getPath = [&dir](const juce::String fileName)
    {
        juce::String path{ dir.file().getFullPathName() + "/" };
        return juce::File{ path + fileName }.getFullPathName();
    };


    EXPECT_EQ(getExt("test1.txt1"), getPath("test1.txt1"));
    EXPECT_EQ(getExt("test2.txt2"), getPath("test2.txt2"));
    EXPECT_EQ(getExt("test3.txt3"), getPath("test3.txt3"));
}

//--------------------------------------------------------------------------------

TEST(DirectoryModel, FileSizeUpdateHandler)
{
    const MessageManagerHelper mm;

    DirTestHelper::InitDirTest();

    DirTestHelper dir;
    dir.createFile("test1.txt1");
    dir.createFile("test2.txt2");
    dir.createFile("test3.txt3");

    vdm::DirectoryModel model;

    vdm::FileSizeUpdateHandler fileSizeUpdateHandler;
    model.addUpdateHandler(fileSizeUpdateHandler);

    model.initialize(dir.file());
    vdm::DirectoryModelSync sync;
    sync.syncModel(model);

    mm.runMessageThread();

    auto getSize = [&model](const juce::String &file)
    {
        return roundToInt(model.getValueTree().getChildWithName(file).getProperty(vdm::FileSizeUpdateHandler::Key));
    };

    EXPECT_EQ(getSize("test1.txt1"), 0);
    EXPECT_EQ(getSize("test2.txt2"), 0);
    EXPECT_EQ(getSize("test3.txt3"), 0);

    {
        juce::File f{ dir.file().getChildFile("test1.txt1") };
        juce::FileOutputStream os{ f };
        os.writeString("asdf");
        os.flush();
    }

    mm.runMessageThread();

    EXPECT_EQ(getSize("test1.txt1"), 5);
}

//--------------------------------------------------------------------------------

TEST(DirectoryModel, FileOrDirUpdateHandler)
{
    const MessageManagerHelper mm;

    DirTestHelper::InitDirTest();

    DirTestHelper dir;
    dir.createFile("test1.txt1");
    dir.createFile("test2.txt2");
    dir.createFile("test3.txt3");

    vdm::DirectoryModel model;

    model.initialize(dir.file());

    mm.runMessageThread();

    auto get = [](juce::ValueTree tree)
    {
        const int k = tree.getProperty(vdm::DirectoryModel::TypeKey);
        return static_cast<vdm::DirectoryModel::Type>(k);
    };

    EXPECT_EQ(get(model.getValueTree()), vdm::DirectoryModel::Type::Dir);
    EXPECT_EQ(get(model.getValueTree().getChildWithName("test1.txt1")), vdm::DirectoryModel::Type::File);
    EXPECT_EQ(get(model.getValueTree().getChildWithName("test2.txt2")), vdm::DirectoryModel::Type::File);
    EXPECT_EQ(get(model.getValueTree().getChildWithName("test3.txt3")), vdm::DirectoryModel::Type::File);
}

//--------------------------------------------------------------------------------

TEST(DirectoryModelSync, SyncMultipleDirectories)
{
    const MessageManagerHelper mm;

    DirTestHelper::InitDirTest();

    DirTestHelper dir;
    DirTestHelper dirA{ dir.createDir("a") };
    DirTestHelper dirB{ dir.createDir("b") };

    vdm::DirectoryModel modelA;
    modelA.initialize(dirA.file());
    vdm::DirectoryModel modelB;
    modelB.initialize(dirB.file());

    vdm::DirectoryModelSync sync;
    sync.syncModel(modelA);
    sync.syncModel(modelB);

    EXPECT_TRUE(sync.isModelSynced(modelA));
    EXPECT_TRUE(sync.isModelSynced(modelB));

    dirA.createFile("a_child_0");
    dirA.createFile("a_child_1");

    dirB.createFile("b_child_0");
    dirB.createFile("b_child_1");

    mm.runMessageThread();

    EXPECT_EQ(modelA.getValueTree().getNumChildren(), 2);
    EXPECT_EQ(modelB.getValueTree().getNumChildren(), 2);

    EXPECT_TRUE(modelA.getValueTree().getChildWithName("a_child_0").isValid());
    EXPECT_TRUE(modelA.getValueTree().getChildWithName("a_child_1").isValid());
    EXPECT_TRUE(modelB.getValueTree().getChildWithName("b_child_0").isValid());
    EXPECT_TRUE(modelB.getValueTree().getChildWithName("b_child_1").isValid());

    sync.unsyncModel(modelA);

    EXPECT_FALSE(sync.isModelSynced(modelA));

    dirA.createFile("a_child_2");

    mm.runMessageThread();

    EXPECT_FALSE(modelA.getValueTree().getChildWithName("a_child_2").isValid());
}

//--------------------------------------------------------------------------------
