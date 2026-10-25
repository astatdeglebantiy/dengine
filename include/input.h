#pragma once

#include <SDL3/SDL.h>
#include <stdint.h>

#define DENGINE_INPUT_KEY_COUNT SDL_SCANCODE_COUNT

typedef struct {
    /* Compatibility aliases for common controls. */
    uint8_t key_w;
    uint8_t key_a;
    uint8_t key_s;
    uint8_t key_d;
    uint8_t key_space;
    uint8_t key_escape;
    uint8_t mouse_left;
    uint8_t mouse_right;

    /* One byte per SDL scancode, including keys not named above. */
    uint8_t keys[DENGINE_INPUT_KEY_COUNT];
    uint32_t mouse_buttons;
    float mouse_x;
    float mouse_y;
    float mouse_delta_x;
    float mouse_delta_y;
    float wheel_x;
    float wheel_y;
} DengineInputState;

void DengineInput_begin_frame(DengineInputState* input);
void DengineInput_process_event(DengineInputState* input, const SDL_Event* event);
