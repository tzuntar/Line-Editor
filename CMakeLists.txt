cmake_minimum_required(VERSION 3.13)
project(Line_Editor)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -lpanel -lncurses")

file(GLOB_RECURSE Line_Editor_Sources
        "src/*.h"
        "src/*.cpp"
        )

add_executable(Line_Editor ${Line_Editor_Sources})
target_link_libraries(Line_Editor panel ncurses)
