cmake_minimum_required(VERSION 3.10)
project(engineProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(source)

# Source and header files
set(SOURCES
        source/platform/window_x11.cpp
        source/platform/event_x11.cpp
        source/platform/renderer.cpp
        tests/flappy.cpp
)
set(HEADERS
        source/platform/window.hpp
        source/platform/event.hpp
        source/platform/renderer.hpp
)

# Main executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE source)
target_link_libraries(${PROJECT_NAME} PRIVATE X11::X11)

# Find dependencies
find_package(X11 REQUIRED)
if(NOT X11_FOUND)
    message(FATAL_ERROR "X11 not found. Please install libx11-dev or equivalent.")
endif()

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# Python binding
pybind11_add_module(platform_engine
        bindings/python_binding.cpp
        source/platform/window_x11.cpp
        source/platform/event_x11.cpp
        source/platform/renderer.cpp
)
target_include_directories(platform_engine PRIVATE source)
target_link_libraries(platform_engine PRIVATE X11::X11)

# Copy the Python module to the python directory
add_custom_command(TARGET platform_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:platform_engine>
        ${CMAKE_SOURCE_DIR}/python/
)