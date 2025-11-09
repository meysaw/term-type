cmake_minimum_required(VERSION 3.16)
project(term-type)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(FTXUI FTXUI_build)
file(GLOB EXAMPLE_SOURCES "src/*.cpp")

add_executable(term-type ${EXAMPLE_SOURCES})
target_link_libraries(term-type
    PRIVATE
        ftxui::screen
        ftxui::dom
        ftxui::component
        
)
