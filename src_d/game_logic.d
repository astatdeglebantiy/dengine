module game_logic;

struct DengineInputState {
    ubyte key_w;
    ubyte key_a;
    ubyte key_s;
    ubyte key_d;
    ubyte key_space;
    ubyte key_escape;
    ubyte mouse_left;
    ubyte mouse_right;
    float mouse_x;
    float mouse_y;
    float mouse_delta_x;
    float mouse_delta_y;
}

private float elapsed_seconds;
private uint frame_count;
private float player_x;
private float player_y;

export extern(C) int dengine_game_init() nothrow @nogc {
    elapsed_seconds = 0.0f;
    frame_count = 0;
    player_x = 0.0f;
    player_y = 0.0f;
    return 0;
}

export extern(C) void dengine_game_update(
    float delta_seconds,
    const(DengineInputState)* input
) nothrow @nogc {
    if (delta_seconds > 0.0f) {
        elapsed_seconds += delta_seconds;
    }

    if (input !is null && delta_seconds > 0.0f) {
        const float movement = delta_seconds;
        if (input.key_w != 0) player_y -= movement;
        if (input.key_s != 0) player_y += movement;
        if (input.key_a != 0) player_x -= movement;
        if (input.key_d != 0) player_x += movement;
        if (input.mouse_left != 0) {
            player_x += input.mouse_delta_x * 0.001f;
            player_y += input.mouse_delta_y * 0.001f;
        }
    }

    frame_count++;
}

export extern(C) void dengine_game_shutdown() nothrow @nogc {
    elapsed_seconds = 0.0f;
    frame_count = 0;
    player_x = 0.0f;
    player_y = 0.0f;
}
