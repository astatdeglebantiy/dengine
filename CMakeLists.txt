cmake_minimum_required(VERSION 3.21)

project(dengine
    VERSION 0.1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(SDL3 CONFIG REQUIRED)

add_executable(dengine
    src_c/main.c
    src_c/window.c
    src_c/renderer.c
)

target_include_directories(dengine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(dengine
    PRIVATE
        SDL3::SDL3
)

# Keep shader assets available from the build directory.
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

if(MSVC)
    target_compile_options(dengine PRIVATE /W4)
else()
    target_compile_options(dengine PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
    )
endif()
