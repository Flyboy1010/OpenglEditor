#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in float texture_id;
layout(location = 2) in vec2 texture_uv;
layout(location = 3) in vec4 color;

uniform mat4 u_view;
uniform mat4 u_proj;

out float v_texture_id;
out vec2 v_texture_uv;
out vec4 v_color;
out vec2 v_screen_pos;

void main() {
    v_texture_id = texture_id;
    v_texture_uv = texture_uv;
    v_color = color;

    mat4 mvp = u_proj * u_view;
    gl_Position = mvp * position;

    v_screen_pos = gl_Position.xy;
}