cmake_minimum_required(VERSION 3.15)
project(atom)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Include paths ----
# Project root so includes like "engine/..." work
include_directories(${CMAKE_SOURCE_DIR})

# External libraries
include_directories(${CMAKE_SOURCE_DIR}/external/glad/include)

# ---- Collect source files ----
file(GLOB_RECURSE ENGINE_SOURCES
    engine/*.cpp
)

file(GLOB_RECURSE PROJECT_SOURCES
    assets/*.cpp
)

set(MAIN_SOURCE main.cpp)
set(GLAD_SRC external/glad/src/glad.c)

# ---- Create executable ----
add_executable(atom
    ${MAIN_SOURCE}
    ${ENGINE_SOURCES}
    ${PROJECT_SOURCES}
    ${GLAD_SRC}
)

# ---- GLFW ----
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(atom glfw)

# ---- OpenGL platform linking ----
if(APPLE)
    target_link_libraries(atom
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
elseif(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(atom
        OpenGL::GL
        dl
        pthread
        X11
        Xrandr
        Xi
    )
endif()