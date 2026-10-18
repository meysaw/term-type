cmake_minimum_required(VERSION 3.20)

project(term-type
    VERSION 0.1.0
    DESCRIPTION "Terminal typing speed test"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Single-config generators leave CMAKE_BUILD_TYPE empty by default, which means
# no optimisation *and* no debug info. Default to Release unless told otherwise.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        Debug Release RelWithDebInfo MinSizeRel)
endif()

include(FetchContent)

# Use an FTXUI that is already installed if there is one, otherwise fetch a
# pinned revision. Pinned on purpose: FTXUI has no stable ABI, so an unpinned
# branch would break builds without warning. Bump this deliberately.
find_package(ftxui CONFIG QUIET)

if(ftxui_FOUND)
    message(STATUS "term-type: using system FTXUI")
else()
    set(FTXUI_VERSION v7.0.3)

    # Only the three libraries are wanted; the rest is upstream's own tooling.
    set(FTXUI_BUILD_DOCS      OFF CACHE BOOL "" FORCE)
    set(FTXUI_BUILD_EXAMPLES  OFF CACHE BOOL "" FORCE)
    set(FTXUI_BUILD_TESTS     OFF CACHE BOOL "" FORCE)
    set(FTXUI_BUILD_MODULES   OFF CACHE BOOL "" FORCE)
    set(FTXUI_ENABLE_COVERAGE OFF CACHE BOOL "" FORCE)
    set(FTXUI_DEV_WARNINGS    OFF CACHE BOOL "" FORCE)
    # Keep upstream's install rules from leaking into `cmake --install`.
    set(FTXUI_ENABLE_INSTALL  OFF CACHE BOOL "" FORCE)

    message(STATUS "term-type: fetching FTXUI ${FTXUI_VERSION}")

    FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
        GIT_TAG        ${FTXUI_VERSION}
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(ftxui)
endif()

# Sources are listed explicitly rather than globbed: file(GLOB) is evaluated at
# configure time only, so a newly added .cpp would be silently ignored.
add_executable(term-type
    src/main.cpp
)

# Only the top of the chain is named: component links dom, and dom links screen,
# both PUBLIC. Listing all three again duplicates them on the linker command
# line, which Apple's linker reports as a warning.
target_link_libraries(term-type
    PRIVATE
        ftxui::component
)
