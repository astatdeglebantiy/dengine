module game_logic;

enum uint DENGINE_INPUT_KEY_COUNT = 512;
enum int KEY_A = 4;
enum int KEY_D = 7;
enum int KEY_S = 22;
enum int KEY_W = 26;
enum int KEY_SPACE = 44;
enum int KEY_ESCAPE = 41;
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
        const float movement = delta_seconds * 0.7f;
        if (input.keys[KEY_W] != 0 || input.keys[KEY_UP] != 0) player_y -= movement;
        if (input.keys[KEY_S] != 0 || input.keys[KEY_DOWN] != 0) player_y += movement;
        if (input.keys[KEY_A] != 0 || input.keys[KEY_LEFT] != 0) player_x -= movement;
        if (input.keys[KEY_D] != 0 || input.keys[KEY_RIGHT] != 0) player_x += movement;
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
