#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <stdbool.h>

#include "input.h"

typedef struct {
    SDL_Window*    sdl_window;
    SDL_GPUDevice* gpu_device;
    int            width;
    int            height;
    bool           is_running;
} Window;

Window Window_create(int width, int height, const char* title);
void   Window_destroy(Window* self);
void   Window_poll_events(Window* self, DengineInputState* input);
