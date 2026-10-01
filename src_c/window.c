#include "window.h"

Window Window_create(int width, int height, const char* title) {
    Window win = {0};
    win.width = width;
    win.height = height;
    win.is_running = true;

    SDL_Init(SDL_INIT_VIDEO);

    win.sdl_window = SDL_CreateWindow(title, width, height, 0);

    win.gpu_device = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV,
        true,
        NULL
    );

    SDL_ClaimWindowForGPUDevice(win.gpu_device, win.sdl_window);

    return win;
}

void Window_poll_events(Window* self) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            self->is_running = false;
        }
    }
}

void Window_destroy(Window* self) {
    if (self->gpu_device) {
        SDL_ReleaseWindowFromGPUDevice(self->gpu_device, self->sdl_window);
        SDL_DestroyGPUDevice(self->gpu_device);
        self->gpu_device = NULL;
    }
    if (self->sdl_window) {
        SDL_DestroyWindow(self->sdl_window);
        self->sdl_window = NULL;
    }
    SDL_Quit();
}