enable_testing()

FetchContent_Declare(
        gtest
        GIT_REPOSITORY "https://github.com/google/googletest.git"
        GIT_TAG "main"
        SOURCE_DIR "${CMAKE_BINARY_DIR}/gtest"
        BINARY_DIR "${CMAKE_BINARY_DIR}/gtest_build"
)
FetchContent_MakeAvailable(gtest)

set(TestSourceFiles
        Tests.cpp
        TestHelpers.cpp
        TestHelpers.h
)

add_executable("Tests" "${TestSourceFiles}")

target_compile_definitions(
        "Tests"
        PRIVATE
        -DTEST_TEMP_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tmp/"
        JUCE_USE_CURL=0
        -DJUCE_WEB_BROWSER=0
)

target_link_libraries("Tests"
        PUBLIC
        gtest
        gmock

        vdm::vdm
        vdm::vdm_directory

        juce::juce_core
        juce::juce_gui_basics
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)