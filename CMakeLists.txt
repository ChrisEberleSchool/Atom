cmake_minimum_required(VERSION 3.15)
project(atom)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect all C++ sources in src/
file(GLOB_RECURSE CPP_SOURCES src/*.cpp)

# glad.c is now in src/glad.c
set(GLAD_SRC ${CMAKE_SOURCE_DIR}/src/glad.c)

# Add executable with C++ sources + glad.c
add_executable(atom ${CPP_SOURCES} ${GLAD_SRC})

# Find GLFW
find_package(glfw3 3.3 REQUIRED)

# Link libraries
target_link_libraries(atom glfw GL dl pthread X11 Xrandr Xi)
