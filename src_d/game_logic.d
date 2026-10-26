module game_logic;

enum uint DENGINE_INPUT_KEY_COUNT = 512;
enum int KEY_A = 4;
enum int KEY_D = 7;
enum int KEY_S = 22;
enum int KEY_W = 26;
enum int KEY_LEFT = 80;
enum int KEY_RIGHT = 79;
enum int KEY_UP = 82;
enum int KEY_DOWN = 81;

struct DengineInputState {
    ubyte key_w;
    ubyte key_a;
    ubyte key_s;
    ubyte key_d;
    ubyte key_space;
    ubyte key_escape;
    ubyte mouse_left;
    ubyte mouse_right;
    ubyte[DENGINE_INPUT_KEY_COUNT] keys;
    uint mouse_buttons;
    float mouse_x;
    float mouse_y;
    float mouse_delta_x;
    float mouse_delta_y;
    float wheel_x;
    float wheel_y;
}

private float player_x;
private float player_y;

export extern(C) uint dengine_game_input_state_size() nothrow @nogc {
    return cast(uint) DengineInputState.sizeof;
}

export extern(C) int dengine_game_init() nothrow @nogc {
    player_x = 0.0f;
    player_y = 0.0f;
    return 0;
}

export extern(C) void dengine_game_update(
    float delta_seconds,
    const(DengineInputState)* input
) nothrow @nogc {
    if (input is null || delta_seconds <= 0.0f) {
        return;
    }

    const float movement = delta_seconds * 0.8f;

    /* GPU normalized coordinates have +Y upward. W/Up therefore increase Y. */
    if (input.keys[KEY_W] != 0 || input.keys[KEY_UP] != 0) player_y += movement;
    if (input.keys[KEY_S] != 0 || input.keys[KEY_DOWN] != 0) player_y -= movement;
    if (input.keys[KEY_A] != 0 || input.keys[KEY_LEFT] != 0) player_x -= movement;
    if (input.keys[KEY_D] != 0 || input.keys[KEY_RIGHT] != 0) player_x += movement;

    /* SDL cursor coordinates grow downward, opposite to GPU normalized Y. */
    if (input.mouse_left != 0) {
        player_x += input.mouse_delta_x * 0.002f;
        player_y -= input.mouse_delta_y * 0.002f;
    }

    /* Keep the demo object inside the visible normalized-device-coordinate area. */
    if (player_x < -0.75f) player_x = -0.75f;
    if (player_x > 0.75f) player_x = 0.75f;
    if (player_y < -0.65f) player_y = -0.65f;
    if (player_y > 0.65f) player_y = 0.65f;
}

export extern(C) void dengine_game_get_player_position(float* x, float* y) nothrow @nogc {
    if (x !is null) *x = player_x;
    if (y !is null) *y = player_y;
}

export extern(C) void dengine_game_shutdown() nothrow @nogc {
    player_x = 0.0f;
    player_y = 0.0f;
}
