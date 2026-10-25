module game_logic;

private float elapsed_seconds;
private uint frame_count;

export extern(C) int dengine_game_init() nothrow @nogc {
    elapsed_seconds = 0.0f;
    frame_count = 0;
    return 0;
}

export extern(C) void dengine_game_update(float delta_seconds) nothrow @nogc {
    if (delta_seconds > 0.0f) {
        elapsed_seconds += delta_seconds;
    }
    frame_count++;
}

export extern(C) void dengine_game_shutdown() nothrow @nogc {
    elapsed_seconds = 0.0f;
    frame_count = 0;
}
