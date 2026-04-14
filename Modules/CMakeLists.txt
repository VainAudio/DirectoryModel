include(FetchContent)

option(BUILD_SHARED_LIBS "Build efsw as a shared library" OFF)
FetchContent_Declare(
		efsw
		GIT_REPOSITORY "https://github.com/SpartanJ/efsw.git"
		SOURCE_DIR "${CMAKE_BINARY_DIR}/efsw"
		BINARY_DIR "${CMAKE_BINARY_DIR}/efsw_build"
)
FetchContent_MakeAvailable(efsw)

juce_add_modules(
    	ALIAS_NAMESPACE vdm

		vdm
		vdm_directory
		vdm_ui
)

target_link_libraries(
		vdm
		INTERFACE
		efsw-static
)