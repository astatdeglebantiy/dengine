cmake_minimum_required(VERSION 4.3)
project(dengine C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

# Packages
find_package(SDL3 REQUIRED)
# only sdl3 xd

set(
        SOURCES
        src_c/main.c
        src_c/window.c
        src_c/renderer.c
)

add_executable(dengine ${SOURCES})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(dengine PRIVATE SDL3::SDL3)
