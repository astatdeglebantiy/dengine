#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int  dengine_game_init(void);
void dengine_game_update(float delta_seconds);
void dengine_game_shutdown(void);

#ifdef __cplusplus
}
#endif
