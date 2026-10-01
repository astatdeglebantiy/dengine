#include "window.h"
#include "renderer.h"

int main(int argc, char* argv[]) {
    Window win = Window_create(800, 600, "Dengine GPU Triangle");
    Renderer renderer = Renderer_create(&win);

    while (win.is_running) {
        Window_poll_events(&win);

        Renderer_draw(&renderer, &win);

        SDL_Delay(16); // ~60 FPS
    }

    Renderer_destroy(&renderer, &win);
    Window_destroy(&win);
    return 0;
}