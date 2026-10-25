#include "input.h"


static void clear_held_inputs(DengineInputState* input) {
    input->key_w = 0;
    input->key_a = 0;
    input->key_s = 0;
    input->key_d = 0;
    input->key_space = 0;
    input->key_escape = 0;
    input->mouse_left = 0;
    input->mouse_right = 0;
}

void DengineInput_begin_frame(DengineInputState* input) {
    if (!input) {
        return;
    }

    input->mouse_delta_x = 0.0f;
    input->mouse_delta_y = 0.0f;
}

void DengineInput_process_event(DengineInputState* input, const SDL_Event* event) {
    if (!input || !event) {
        return;
    }

    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP: {
            const uint8_t pressed = event->type == SDL_EVENT_KEY_DOWN ? 1u : 0u;
            switch (event->key.scancode) {
                case SDL_SCANCODE_W: input->key_w = pressed; break;
                case SDL_SCANCODE_A: input->key_a = pressed; break;
                case SDL_SCANCODE_S: input->key_s = pressed; break;
                case SDL_SCANCODE_D: input->key_d = pressed; break;
                case SDL_SCANCODE_SPACE: input->key_space = pressed; break;
                case SDL_SCANCODE_ESCAPE: input->key_escape = pressed; break;
                default: break;
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            const uint8_t pressed = event->type == SDL_EVENT_MOUSE_BUTTON_DOWN ? 1u : 0u;
            if (event->button.button == SDL_BUTTON_LEFT) {
                input->mouse_left = pressed;
            } else if (event->button.button == SDL_BUTTON_RIGHT) {
                input->mouse_right = pressed;
            }
            break;
        }
        case SDL_EVENT_MOUSE_MOTION:
            input->mouse_x = event->motion.x;
            input->mouse_y = event->motion.y;
            input->mouse_delta_x += event->motion.xrel;
            input->mouse_delta_y += event->motion.yrel;
            break;
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            clear_held_inputs(input);
            break;
        default:
            break;
    }
}
