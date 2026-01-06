cmake_minimum_required(VERSION 3.24)

# TODO figure this out
# https://stackoverflow.com/questions/47087237/cmake-and-ninja-rebuild-unnecessary-files
option(LINUX_REBUILD_WORKAROUND "ignore system headers for detecting dirty files" OFF)
if(LINUX_REBUILD_WORKAROUND)
    file(
            WRITE "${CMAKE_BINARY_DIR}/GNUMakeRulesOverwrite.cmake"
            "STRING(REPLACE \"-MD\" \"-MMD\" CMAKE_DEPFILE_FLAGS_C \"\${CMAKE_DEPFILE_FLAGS_C}\")\n"
            "STRING(REPLACE \"-MD\" \"-MMD\" CMAKE_DEPFILE_FLAGS_CXX \"\${CMAKE_DEPFILE_FLAGS_CXX}\")\n"
    )
    set(CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_BINARY_DIR}/GNUMakeRulesOverwrite.cmake" CACHE INTERNAL "")
endif()

project(VainDirectoryModel VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        juce
        GIT_REPOSITORY "https://github.com/juce-framework/JUCE.git"
        GIT_TAG "8.0.12"
        SOURCE_DIR "${CMAKE_BINARY_DIR}/juce"
        BINARY_DIR "${CMAKE_BINARY_DIR}/juce_build"
)
FetchContent_MakeAvailable(juce)

add_subdirectory(Modules)
add_subdirectory(Demo)
add_subdirectory(Tests)
