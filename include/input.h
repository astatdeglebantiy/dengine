#pragma once

#include <SDL3/SDL.h>
#include <stdint.h>

typedef struct {
    uint8_t key_w;
    uint8_t key_a;
    uint8_t key_s;
    uint8_t key_d;
    uint8_t key_space;
    uint8_t key_escape;
    uint8_t mouse_left;
    uint8_t mouse_right;
    float mouse_x;
    float mouse_y;
    float mouse_delta_x;
    float mouse_delta_y;
} DengineInputState;

void DengineInput_begin_frame(DengineInputState* input);
void DengineInput_process_event(DengineInputState* input, const SDL_Event* event);
