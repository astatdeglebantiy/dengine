# Dengine

Dengine is an experimental, cross-platform game-engine starter built with **C23**, **D**, **SDL3**, and the SDL GPU API. The current prototype opens a window, renders a GPU triangle, and loads game logic from a D shared library.

The project is an engine template, not a finished editor or a production-ready engine. Linux and Windows are the initial targets.

## Current architecture

- **C23** — application lifecycle, SDL3 window/events, GPU rendering, and the C ABI boundary.
- **D** — game logic, compiled as a shared library and loaded by the C host through SDL3's dynamic-library API.
- **SDL3 GPU** — rendering with SPIR-V shaders.
- **CMake + Ninja** — cross-platform configuration and build.
- **GitHub Actions** — Linux and Windows build checks.

The D module exports three functions:

- `dengine_game_init()`
- `dengine_game_update(float delta_seconds, const DengineInputState* input)`
- `dengine_game_shutdown()`

## Input

The input state records every SDL keyboard scancode, not just a fixed handful of keys. This includes letters, digits, function keys, arrows, modifiers, navigation keys, and other keys supported by the SDL version and keyboard layout. Common controls (W/A/S/D, Space, Escape) also have compatibility aliases.

Mouse state includes button flags, cursor coordinates, per-frame relative motion, and per-frame scroll-wheel deltas. Held keyboard and mouse state is cleared when the window loses focus. The sample D logic demonstrates movement with WASD or arrow keys and mouse dragging.

Edit `src_d/game_logic.d` to start changing game logic. Keep exported function names and signatures compatible with `include/game_logic.h` and `src_c/game_logic.c` unless you intentionally update both sides of the interface.

## Requirements

Install the following tools:

- A C23-capable C compiler
- CMake 3.21 or newer
- Ninja
- SDL3 development files with the SDL3 CMake package
- LDC (`ldc2`) for compiling D
- Vulkan shader tools providing `glslc` or `glslangValidator`

A Vulkan-capable graphics driver is required to run the current renderer.

### Arch Linux

```sh
sudo pacman -S --needed base-devel cmake ninja sdl3 ldc shaderc
```

### Windows

Install Visual Studio Build Tools with a C compiler, CMake, Ninja, LDC, SDL3 development files, and the Vulkan SDK (including shader tools). Run the following commands from a Developer PowerShell or a terminal where those tools are on `PATH`.

## Build and run

From the repository root:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/dengine
```

On Windows:

```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
.\build\dengine.exe
```

If SDL3 is installed in a custom prefix, pass it during configuration, for example:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/path/to/sdl3
```

## Repository layout

```text
include/       C interfaces and shared engine types
src_c/         C application, SDL3 window, renderer, and D module loader
src_d/         D game-logic module
shaders/       GLSL shader sources
.github/       CI workflow for Linux and Windows
CMakeLists.txt Build configuration
```

## Status and next steps

The engine is an early foundation. A general-purpose scene system, asset management, 2D/3D feature selection, physics modules, packaging, visible rendering of the D-controlled game state, and a template-update workflow are not implemented yet. They should be introduced incrementally behind clear interfaces, with working builds preserved on Linux and Windows.

## CI

Pull requests and pushes to `master` build the project on Linux and Windows using GitHub Actions.
