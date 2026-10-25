#include "input.h"

#include <stdio.h>

#define CHECK(condition) do { \
    if (!(condition)) { \
        fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition); \
        return 1; \
    } \
} while (0)

int main(void) {
    DengineInputState input = {0};
    SDL_Event event = {0};

    /* Keyboard: arbitrary scancodes and compatibility aliases. */
    event.type = SDL_EVENT_KEY_DOWN;
    event.key.scancode = SDL_SCANCODE_F12;
    DengineInput_process_event(&input, &event);
    CHECK(input.keys[SDL_SCANCODE_F12] == 1u);

    event = (SDL_Event){0};
    event.type = SDL_EVENT_KEY_DOWN;
    event.key.scancode = SDL_SCANCODE_W;
    DengineInput_process_event(&input, &event);
    CHECK(input.keys[SDL_SCANCODE_W] == 1u);
    CHECK(input.key_w == 1u);

    event.type = SDL_EVENT_KEY_UP;
    DengineInput_process_event(&input, &event);
    CHECK(input.keys[SDL_SCANCODE_W] == 0u);
    CHECK(input.key_w == 0u);

    /* Mouse buttons and pointer motion. */
    event = (SDL_Event){0};
    event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
    event.button.button = SDL_BUTTON_LEFT;
    DengineInput_process_event(&input, &event);
    CHECK(input.mouse_left == 1u);
    CHECK((input.mouse_buttons & SDL_BUTTON_LMASK) != 0u);

    event = (SDL_Event){0};
    event.type = SDL_EVENT_MOUSE_MOTION;
    event.motion.x = 123.0f;
    event.motion.y = 45.0f;
    event.motion.xrel = 3.0f;
    event.motion.yrel = -2.0f;
    event.motion.state = SDL_BUTTON_LMASK;
    DengineInput_process_event(&input, &event);
    CHECK(input.mouse_x == 123.0f);
    CHECK(input.mouse_y == 45.0f);
    CHECK(input.mouse_delta_x == 3.0f);
    CHECK(input.mouse_delta_y == -2.0f);
    CHECK(input.mouse_left == 1u);

    /* Wheel events accumulate until the next frame begins. */
    event = (SDL_Event){0};
    event.type = SDL_EVENT_MOUSE_WHEEL;
    event.wheel.x = 1.0f;
    event.wheel.y = -2.0f;
    DengineInput_process_event(&input, &event);
    DengineInput_process_event(&input, &event);
    CHECK(input.wheel_x == 2.0f);
    CHECK(input.wheel_y == -4.0f);

    DengineInput_begin_frame(&input);
    CHECK(input.mouse_delta_x == 0.0f);
    CHECK(input.mouse_delta_y == 0.0f);
    CHECK(input.wheel_x == 0.0f);
    CHECK(input.wheel_y == 0.0f);
    CHECK(input.keys[SDL_SCANCODE_F12] == 1u);

    /* Losing focus must release all held keys and mouse buttons. */
    event = (SDL_Event){0};
    event.type = SDL_EVENT_WINDOW_FOCUS_LOST;
    DengineInput_process_event(&input, &event);
    CHECK(input.keys[SDL_SCANCODE_F12] == 0u);
    CHECK(input.keys[SDL_SCANCODE_W] == 0u);
    CHECK(input.key_w == 0u);
    CHECK(input.mouse_buttons == 0u);
    CHECK(input.mouse_left == 0u);
    CHECK(input.mouse_right == 0u);

    puts("All input-state tests passed.");
    return 0;
}
