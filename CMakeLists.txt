cmake_minimum_required(VERSION 3.21)

project(dengine
    VERSION 0.1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(SDL3 CONFIG REQUIRED)
find_program(GLSLC_EXECUTABLE NAMES glslc glslangValidator REQUIRED)

set(SHADER_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")
file(MAKE_DIRECTORY "${SHADER_OUTPUT_DIR}")

if(GLSLC_EXECUTABLE MATCHES "glslangValidator")
    set(SHADER_COMPILER_FLAGS -V)
else()
    set(SHADER_COMPILER_FLAGS)
endif()

set(SPIRV_SHADERS
    "${SHADER_OUTPUT_DIR}/triangle_vert.spv"
    "${SHADER_OUTPUT_DIR}/triangle_frag.spv"
)

add_custom_command(
    OUTPUT "${SHADER_OUTPUT_DIR}/triangle_vert.spv"
    COMMAND "${GLSLC_EXECUTABLE}" ${SHADER_COMPILER_FLAGS}
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/triangle.vert"
        -o "${SHADER_OUTPUT_DIR}/triangle_vert.spv"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/shaders/triangle.vert"
    COMMENT "Compiling vertex shader to SPIR-V"
    VERBATIM
)

add_custom_command(
    OUTPUT "${SHADER_OUTPUT_DIR}/triangle_frag.spv"
    COMMAND "${GLSLC_EXECUTABLE}" ${SHADER_COMPILER_FLAGS}
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/triangle.frag"
        -o "${SHADER_OUTPUT_DIR}/triangle_frag.spv"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/shaders/triangle.frag"
    COMMENT "Compiling fragment shader to SPIR-V"
    VERBATIM
)

add_custom_target(dengine_shaders DEPENDS ${SPIRV_SHADERS})

add_executable(dengine
    src_c/main.c
    src_c/window.c
    src_c/renderer.c
)

add_dependencies(dengine dengine_shaders)

target_include_directories(dengine
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(dengine PRIVATE SDL3::SDL3)

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
