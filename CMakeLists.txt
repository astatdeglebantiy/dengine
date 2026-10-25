cmake_minimum_required(VERSION 3.21)

project(dengine VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include(CTest)

find_package(SDL3 CONFIG REQUIRED)
find_program(LDC2_EXECUTABLE NAMES ldc2 ldc2.exe REQUIRED)
find_program(
    GLSLC_EXECUTABLE
    NAMES glslc glslangValidator
    HINTS
        "$ENV{VULKAN_SDK}/Bin"
        "$ENV{VULKAN_SDK}/bin"
        "$ENV{VULKAN_SDK}/Bin32"
        "$ENV{GITHUB_WORKSPACE}/_vulkan_build/tools/bin"
        "${CMAKE_CURRENT_SOURCE_DIR}/_vulkan_build/tools/bin"
    REQUIRED
)

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

set(D_GAME_LOGIC_LIBRARY
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}game_logic${CMAKE_SHARED_LIBRARY_SUFFIX}"
)

add_custom_command(
    OUTPUT "${D_GAME_LOGIC_LIBRARY}"
    COMMAND "${LDC2_EXECUTABLE}"
        -shared
        "-of=${D_GAME_LOGIC_LIBRARY}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src_d/game_logic.d"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src_d/game_logic.d"
    COMMENT "Building D game-logic shared library"
    VERBATIM
)

add_custom_target(dengine_game_logic DEPENDS "${D_GAME_LOGIC_LIBRARY}")

add_executable(dengine
    src_c/main.c
    src_c/window.c
    src_c/input.c
    src_c/renderer.c
    src_c/game_logic.c
)

add_dependencies(dengine dengine_shaders dengine_game_logic)

target_include_directories(dengine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_definitions(dengine PRIVATE
    DENGINE_GAME_LOGIC_LIBRARY_PATH="${D_GAME_LOGIC_LIBRARY}"
    DENGINE_SHADER_DIR="${SHADER_OUTPUT_DIR}"
)
target_link_libraries(dengine PRIVATE SDL3::SDL3)

if(MSVC)
    target_compile_options(dengine PRIVATE /W4)
else()
    target_compile_options(dengine PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

if(BUILD_TESTING)
    add_executable(dengine_input_tests
        tests/input_test.c
        src_c/input.c
    )
    target_include_directories(dengine_input_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_link_libraries(dengine_input_tests PRIVATE SDL3::SDL3)
    if(MSVC)
        target_compile_options(dengine_input_tests PRIVATE /W4)
    else()
        target_compile_options(dengine_input_tests PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
    endif()
    add_test(NAME dengine_input_tests COMMAND dengine_input_tests)
endif()
