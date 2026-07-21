juce_add_gui_app(Demo PRODUCT_NAME "Demo")

target_compile_definitions(
        Demo
        PUBLIC
        -DPROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}/"
        -DJUCE_USE_CURL=0
        -DJUCE_WEB_BROWSER=0
)

target_sources(
        Demo
        PRIVATE
        Main.cpp
        MainComponent.cpp
        DirectoryModelDemo/DirectoryModelDemoComponent.cpp
        DirectoryModelDemo/TreeView/DirectoryModelDemoTreeView.cpp
        DirectoryModelDemo/SelectionViewer.cpp
)

target_link_libraries(
        Demo
        PRIVATE

        vdm::vdm
        vdm::vdm_directory
        vdm::vdm_ui

        juce::juce_core
        juce::juce_dsp
        juce::juce_data_structures
        juce::juce_audio_processors
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
