#include "renderer.h"
#include "window.h"

int main(void) {
    Window win = Window_create(800, 600, "Dengine GPU Triangle");
    if (!win.is_running) {
        Window_destroy(&win);
        return 1;
    }

    Renderer renderer = Renderer_create(&win);
    if (!renderer.pipeline || !renderer.vertex_buffer) {
        SDL_Log("Failed to create renderer resources.");
        Renderer_destroy(&renderer, &win);
        Window_destroy(&win);
        return 1;
    }

    while (win.is_running) {
        Window_poll_events(&win);
        Renderer_draw(&renderer, &win);
        SDL_Delay(16);
    }

    Renderer_destroy(&renderer, &win);
    Window_destroy(&win);
    return 0;
}
