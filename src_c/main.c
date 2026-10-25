#include "game_logic.h"
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

    if (dengine_game_init() != 0) {
        SDL_Log("Failed to initialize D game logic.");
        Renderer_destroy(&renderer, &win);
        Window_destroy(&win);
        return 1;
    }

    Uint64 previous_ticks = SDL_GetTicks();
    while (win.is_running) {
        Window_poll_events(&win);

        const Uint64 current_ticks = SDL_GetTicks();
        const float delta_seconds =
            (float)(current_ticks - previous_ticks) / 1000.0f;
        previous_ticks = current_ticks;

        dengine_game_update(delta_seconds);
        Renderer_draw(&renderer, &win);
        SDL_Delay(16);
    }

    dengine_game_shutdown();
    Renderer_destroy(&renderer, &win);
    Window_destroy(&win);
    return 0;
}
