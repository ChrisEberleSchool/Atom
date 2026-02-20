cmake_minimum_required(VERSION 3.15)
project(atom)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE CPP_SOURCES src/*.cpp)
set(GLAD_SRC ${CMAKE_SOURCE_DIR}/src/glad.c)

add_executable(atom ${CPP_SOURCES} ${GLAD_SRC})

find_package(glfw3 3.3 REQUIRED)

target_link_libraries(atom glfw)

# Platform-specific linking
if(APPLE)
    target_link_libraries(atom
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
elseif(UNIX)
    target_link_libraries(atom
        GL dl pthread X11 Xrandr Xi
    )
endif()
