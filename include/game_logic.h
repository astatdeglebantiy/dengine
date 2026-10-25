#pragma once

#include <SDL3/SDL.h>
#include "input.h"

typedef int  (*DengineGameInit)(void);
typedef void (*DengineGameUpdate)(float delta_seconds, const DengineInputState* input);
typedef uint32_t (*DengineGameInputStateSize)(void);
typedef void (*DengineGameGetPlayerPosition)(float* x, float* y);
typedef void (*DengineGameShutdown)(void);

typedef struct {
    void* module;
    DengineGameInit init;
    DengineGameUpdate update;
    DengineGameInputStateSize input_state_size;
    DengineGameGetPlayerPosition get_player_position;
    DengineGameShutdown shutdown;
} DengineGameLogic;

bool DengineGameLogic_load(DengineGameLogic* game);
void DengineGameLogic_unload(DengineGameLogic* game);
