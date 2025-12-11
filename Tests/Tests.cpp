#include <gtest/gtest.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "DirectoryModelTests.h"
#include "SelectionModelTests.h"
#include "TraversalTests.h"

//--------------------------------------------------------------------------------

namespace juce
{
    extern void initialiseNSApplication();
}

//--------------------------------------------------------------------------------

class TestThread : public juce::Thread
{
public:
    TestThread()
        : juce::Thread("test_thread")
    {}

    int returnValue{ -1 };
    void run() override
    {
        returnValue = RUN_ALL_TESTS();
        juce::MessageManager::getInstance()->stopDispatchLoop();
    }
};

//--------------------------------------------------------------------------------

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

#ifdef JUCE_MAC
    juce::initialiseNSApplication();
#endif

    const juce::ScopedJuceInitialiser_GUI gui;
    juce::ignoreUnused(gui);

    TestThread testThread;
    juce::MessageManager::callAsync([&testThread]()
    {
        testThread.startThread();
    });

    juce::MessageManager::getInstance()->runDispatchLoop();

    return testThread.returnValue;
}

//--------------------------------------------------------------------------------
