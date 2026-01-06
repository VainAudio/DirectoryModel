#pragma once
#include <juce_events/juce_events.h>

//--------------------------------------------------------------------------------

class MessageManagerHelper
{
public:
    MessageManagerHelper();
    ~MessageManagerHelper();

    void runMessageThread(int milliseconds = 1000) const;

private:
    juce::MessageManager::Lock m_lock;
};

//--------------------------------------------------------------------------------

class DirTestHelper
{
public:
    DirTestHelper();
    ~DirTestHelper();

    static void InitDirTest(); // TODO do need this?

    juce::File createFile(const juce::String &name);
    void deleteFile(const juce::String &name);

    DirTestHelper createDir(const juce::String &name);
    void deleteDir(const juce::String &name);

    juce::File file();

private:
    DirTestHelper(const DirTestHelper &) = delete;
    DirTestHelper(DirTestHelper &&) = default;
    explicit DirTestHelper(juce::File child);

    juce::File m_root{ TEST_TEMP_DIR };
};
//--------------------------------------------------------------------------------
