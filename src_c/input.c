#include "input.h"

static void sync_common_keys(DengineInputState* input) {
    input->key_w = input->keys[SDL_SCANCODE_W];
    input->key_a = input->keys[SDL_SCANCODE_A];
    input->key_s = input->keys[SDL_SCANCODE_S];
    input->key_d = input->keys[SDL_SCANCODE_D];
    input->key_space = input->keys[SDL_SCANCODE_SPACE];
    input->key_escape = input->keys[SDL_SCANCODE_ESCAPE];
    input->mouse_left = (input->mouse_buttons & SDL_BUTTON_LMASK) != 0;
    input->mouse_right = (input->mouse_buttons & SDL_BUTTON_RMASK) != 0;
}

static void clear_held_inputs(DengineInputState* input) {
    SDL_memset(input->keys, 0, sizeof(input->keys));
    input->mouse_buttons = 0;
    sync_common_keys(input);
}

void DengineInput_begin_frame(DengineInputState* input) {
    if (!input) {
        return;
    }

    input->mouse_delta_x = 0.0f;
    input->mouse_delta_y = 0.0f;
    input->wheel_x = 0.0f;
    input->wheel_y = 0.0f;
}

void DengineInput_process_event(DengineInputState* input, const SDL_Event* event) {
    if (!input || !event) {
        return;
    }

    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP: {
            const SDL_Scancode scancode = event->key.scancode;
            if (scancode > SDL_SCANCODE_UNKNOWN && scancode < SDL_SCANCODE_COUNT) {
                input->keys[scancode] = event->type == SDL_EVENT_KEY_DOWN ? 1u : 0u;
                sync_common_keys(input);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            const uint32_t mask = SDL_BUTTON_MASK(event->button.button);
            if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                input->mouse_buttons |= mask;
            } else {
                input->mouse_buttons &= ~mask;
            }
            sync_common_keys(input);
            break;
        }
        case SDL_EVENT_MOUSE_MOTION:
            input->mouse_x = event->motion.x;
            input->mouse_y = event->motion.y;
            input->mouse_delta_x += event->motion.xrel;
            input->mouse_delta_y += event->motion.yrel;
            input->mouse_buttons = event->motion.state;
            sync_common_keys(input);
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            input->wheel_x += event->wheel.x;
            input->wheel_y += event->wheel.y;
            break;
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            clear_held_inputs(input);
            break;
        default:
            break;
    }
}
