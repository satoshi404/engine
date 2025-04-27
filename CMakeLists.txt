cmake_minimum_required(VERSION 3.31)
project("engine" VERSION 1.0)

include_directories(source)

set( SOURCES
 # source/main.cpp
  # ======= core/launcher =======
  #source/core/launcher/launcher.cpp
  # ======= platform =======
  source/platform/window_x11.cpp
  source/platform/event_x11.cpp
   source/platform/renderer.cpp
    #    source/platform/game.cpp
       #tests/test.cpp
        tests/flappy.cpp

)

set( HEADERS
# ======= core/launcher =======
#source/core/launcher/launcher.hpp
  # ======= platform =======
  source/platform/window.hpp
  source/platform/event.hpp
   source/platform/renderer.hpp
   #     source/platform/game.hpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Specify include directories for the target
target_include_directories(${PROJECT_NAME} PRIVATE source)

# Find X11 library
find_package(X11 REQUIRED)
if(NOT X11_FOUND)
    message(FATAL_ERROR "X11 not found. Please install libx11-dev or equivalent.")
endif()

# Link X11 library to the executable
target_link_libraries(${PROJECT_NAME} PRIVATE X11::X11)
