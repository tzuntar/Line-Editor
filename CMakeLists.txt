cmake_minimum_required(VERSION 3.13)
project(lineedit)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -lpanel -lncurses")

file(GLOB_RECURSE Line_Editor_Sources
        "src/*.h"
        "src/*.cpp"
        )

add_executable(lineedit ${Line_Editor_Sources})
target_link_libraries(lineedit panel ncurses)
