#include "game_logic.h"
#include "renderer.h"
#include "window.h"

int main(void) {
    Window win = Window_create(800, 600, "Dengine - WASD / Arrows to move");
    if (!win.is_running) {
        Window_destroy(&win);
        return 1;
    }

    DengineGameLogic game = {0};
    if (!DengineGameLogic_load(&game)) {
        SDL_Log("Failed to load D game logic module: %s", SDL_GetError());
        Window_destroy(&win);
        return 1;
    }

    Renderer renderer = Renderer_create(&win);
    if (!renderer.pipeline || !renderer.vertex_buffer) {
        SDL_Log("Failed to create renderer resources.");
        Renderer_destroy(&renderer, &win);
        DengineGameLogic_unload(&game);
        Window_destroy(&win);
        return 1;
    }

    if (game.init() != 0) {
        SDL_Log("Failed to initialize D game logic.");
        Renderer_destroy(&renderer, &win);
        DengineGameLogic_unload(&game);
        Window_destroy(&win);
        return 1;
    }

    SDL_Log("Dengine demo: move the triangle with WASD or arrow keys; hold left mouse and drag.");
    DengineInputState input = {0};
    Uint64 previous_ticks = SDL_GetTicks();
    while (win.is_running) {
        DengineInput_begin_frame(&input);
        Window_poll_events(&win, &input);

        const Uint64 current_ticks = SDL_GetTicks();
        float delta_seconds = (float)(current_ticks - previous_ticks) / 1000.0f;
        previous_ticks = current_ticks;

        /* Avoid huge simulation steps after stalls, debugger pauses, or window drags. */
        if (delta_seconds > 0.05f) {
            delta_seconds = 0.05f;
        }

        game.update(delta_seconds, &input);

        float player_x = 0.0f;
        float player_y = 0.0f;
        game.get_player_position(&player_x, &player_y);
        Renderer_draw(&renderer, &win, player_x, player_y);
        SDL_Delay(1);
    }

    game.shutdown();
    Renderer_destroy(&renderer, &win);
    DengineGameLogic_unload(&game);
    Window_destroy(&win);
    return 0;
}
