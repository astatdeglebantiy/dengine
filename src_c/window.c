#include "window.h"

Window Window_create(int width, int height, const char* title) {
    Window win = {0};
    win.width = width;
    win.height = height;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return win;
    }

    win.sdl_window = SDL_CreateWindow(title, width, height, 0);
    if (!win.sdl_window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return win;
    }

    win.gpu_device = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV,
        true,
        NULL
    );
    if (!win.gpu_device) {
        SDL_Log("SDL_CreateGPUDevice failed: %s", SDL_GetError());
        SDL_DestroyWindow(win.sdl_window);
        win.sdl_window = NULL;
        SDL_Quit();
        return win;
    }

    if (!SDL_ClaimWindowForGPUDevice(win.gpu_device, win.sdl_window)) {
        SDL_Log("SDL_ClaimWindowForGPUDevice failed: %s", SDL_GetError());
        SDL_DestroyGPUDevice(win.gpu_device);
        SDL_DestroyWindow(win.sdl_window);
        win.gpu_device = NULL;
        win.sdl_window = NULL;
        SDL_Quit();
        return win;
    }

    win.is_running = true;
    return win;
}

void Window_poll_events(Window* self) {
    if (!self) {
        return;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            self->is_running = false;
        }
    }
}

void Window_destroy(Window* self) {
    if (!self) {
        return;
    }

    if (self->gpu_device) {
        if (self->sdl_window) {
            SDL_ReleaseWindowFromGPUDevice(self->gpu_device, self->sdl_window);
        }
        SDL_DestroyGPUDevice(self->gpu_device);
        self->gpu_device = NULL;
    }

    if (self->sdl_window) {
        SDL_DestroyWindow(self->sdl_window);
        self->sdl_window = NULL;
    }

    self->is_running = false;
    SDL_Quit();
}
