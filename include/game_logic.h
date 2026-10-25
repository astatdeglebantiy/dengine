#pragma once

#include <SDL3/SDL.h>

typedef int  (*DengineGameInit)(void);
typedef void (*DengineGameUpdate)(float delta_seconds);
typedef void (*DengineGameShutdown)(void);

typedef struct {
    void* module;
    DengineGameInit init;
    DengineGameUpdate update;
    DengineGameShutdown shutdown;
} DengineGameLogic;

bool DengineGameLogic_load(DengineGameLogic* game);
void DengineGameLogic_unload(DengineGameLogic* game);
