#include "TestHelpers.h"

//--------------------------------------------------------------------------------

MessageManagerHelper::MessageManagerHelper()
{
    m_lock.enter();
}

//--------------------------------------------------------------------------------

MessageManagerHelper::~MessageManagerHelper()
{
    m_lock.exit();
}

//--------------------------------------------------------------------------------

void MessageManagerHelper::runMessageThread(int milliseconds) const
{
    m_lock.exit();
    juce::Thread::sleep(milliseconds);
    m_lock.enter();
}

//--------------------------------------------------------------------------------

DirTestHelper::DirTestHelper()
{
    if (!m_root.exists())
    {
        auto result = m_root.createDirectory();
        jassert(result.wasOk());
    }
}

//--------------------------------------------------------------------------------

DirTestHelper::~DirTestHelper()
{
    auto result = m_root.deleteRecursively();
    jassert(result);
}

//--------------------------------------------------------------------------------

void DirTestHelper::InitDirTest()
{
    juce::File root{ TEST_TEMP_DIR };
    if (root.exists())
    {
        const auto r = root.deleteRecursively();
        jassert(r);
        // waiting here makes sure the directory is actually gone and we get the right callbacks later
        juce::Thread::sleep(5000);
    }
}

//--------------------------------------------------------------------------------

juce::File DirTestHelper::createFile(const juce::String& name)
{
    auto file = m_root.getChildFile(name);
    if (!file.exists())
    {
        auto result = file.create();
        if (!result.wasOk())
        {
            jassertfalse;
            return {};
        }
    }

    return file;
}

//--------------------------------------------------------------------------------

void DirTestHelper::deleteFile(const juce::String& name)
{
    auto file = m_root.getChildFile(name);
    if (file.exists())
    {
        auto result = file.deleteFile();
        jassert(result);
    }
    else
        jassertfalse;
}

//--------------------------------------------------------------------------------

DirTestHelper DirTestHelper::createDir(const juce::String& name)
{
    juce::File file{ m_root.getFullPathName() + "/" + name + "/" };
    if (!file.exists())
    {
        auto result = file.createDirectory();
        jassert(result.wasOk());
    }

    return file.exists() ? DirTestHelper{ file } : DirTestHelper{ m_root };
}

//--------------------------------------------------------------------------------

void DirTestHelper::deleteDir(const juce::String& name)
{
    auto file = m_root.getChildFile(name);
    jassert(file.isDirectory());
    if (file.exists())
    {
        auto result = file.deleteRecursively();
        jassert(result);
    }
    else
        jassertfalse;
}

//--------------------------------------------------------------------------------

juce::File DirTestHelper::file()

{
    return m_root;
}

//--------------------------------------------------------------------------------

DirTestHelper::DirTestHelper(juce::File child)
    : m_root(child)
{}

//--------------------------------------------------------------------------------
