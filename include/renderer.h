#pragma once
#include "window.h"
#include "math3d.h"

typedef struct {
    SDL_GPUGraphicsPipeline* pipeline;
    SDL_GPUBuffer*           vertex_buffer;
} Renderer;

Renderer Renderer_create(Window* win);
void     Renderer_draw(Renderer* self, Window* win, float player_x, float player_y);
void     Renderer_destroy(Renderer* self, Window* win);
