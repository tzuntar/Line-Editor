cmake_minimum_required(VERSION 3.13)
project(lineedit)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Wall -lpanel -lncurses")

file(GLOB_RECURSE Line_Editor_Sources
        "src/*.h"
        "src/*.cpp"
        )

add_executable(lineedit ${Line_Editor_Sources} src/display/Window.cpp src/display/Window.h src/display/windows/TextWindow.cpp src/display/windows/TextWindow.h src/display/windows/ConsoleWindow.cpp src/display/windows/ConsoleWindow.h src/display/Utilities.h src/display/Message.cpp src/display/Message.cpp src/display/Message.h)
target_link_libraries(lineedit panel ncurses)
