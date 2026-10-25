#include "game_logic.h"

#ifndef DENGINE_GAME_LOGIC_LIBRARY_PATH
#error DENGINE_GAME_LOGIC_LIBRARY_PATH must be set by CMake
#endif

bool DengineGameLogic_load(DengineGameLogic* game) {
    if (!game) {
        return false;
    }

    *game = (DengineGameLogic){0};
    game->module = SDL_LoadObject(DENGINE_GAME_LOGIC_LIBRARY_PATH);
    if (!game->module) {
        return false;
    }

    game->init = (DengineGameInit)SDL_LoadFunction(game->module, "dengine_game_init");
    game->update = (DengineGameUpdate)SDL_LoadFunction(game->module, "dengine_game_update");
    game->input_state_size = (DengineGameInputStateSize)SDL_LoadFunction(
        game->module, "dengine_game_input_state_size"
    );
    game->get_player_position = (DengineGameGetPlayerPosition)SDL_LoadFunction(
        game->module, "dengine_game_get_player_position"
    );
    game->shutdown = (DengineGameShutdown)SDL_LoadFunction(game->module, "dengine_game_shutdown");

    if (!game->init || !game->update || !game->input_state_size ||
        !game->get_player_position || !game->shutdown) {
        SDL_Log("D game module is missing a required exported function.");
        DengineGameLogic_unload(game);
        return false;
    }

    const uint32_t d_input_size = game->input_state_size();
    if (d_input_size != sizeof(DengineInputState)) {
        SDL_Log(
            "C/D input ABI mismatch: C expects %zu bytes, D reports %u bytes.",
            sizeof(DengineInputState), d_input_size
        );
        DengineGameLogic_unload(game);
        return false;
    }

    return true;
}

void DengineGameLogic_unload(DengineGameLogic* game) {
    if (!game) {
        return;
    }
    if (game->module) {
        SDL_UnloadObject(game->module);
    }
    *game = (DengineGameLogic){0};
}
