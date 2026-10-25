#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;

layout(set = 1, binding = 0) uniform PlayerTransform {
    vec2 offset;
} player_transform;

layout(location = 0) out vec4 out_color;

void main() {
    gl_Position = vec4(in_position.xy + player_transform.offset, in_position.z, 1.0);
    out_color = in_color;
}
