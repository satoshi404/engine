cmake_minimum_required(VERSION 3.10)
project(engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(X11 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# Main engine executable
add_executable(engine
        source/main.cpp
        source/platform/window.cpp
        source/platform/event.cpp
        source/platform/renderer.cpp
        source/platform/game.cpp
)
target_include_directories(engine PRIVATE source)
target_link_libraries(engine PRIVATE X11::X11)

# Python binding
pybind11_add_module(platform
        bindings/platform_binding.cpp
        source/platform/window.cpp
        source/platform/event.cpp
        source/platform/renderer.cpp
        source/platform/game.cpp
)
target_include_directories(platform PRIVATE source)
target_link_libraries(platform PRIVATE X11::X11)