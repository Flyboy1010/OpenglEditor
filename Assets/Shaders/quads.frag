#version 330 core

layout(location = 0) out vec4 outputColor;

uniform sampler2D u_textures[32];

in float v_texture_id;
in vec2 v_texture_uv;
in vec4 v_color;
in vec2 v_screen_pos;

void main() {
    int index = int(v_texture_id);

    switch (index)
    {
        case 0 : outputColor = texture(u_textures[0],  v_texture_uv) * v_color; break;
        case 1 : outputColor = texture(u_textures[1],  v_texture_uv) * v_color; break;
        case 2 : outputColor = texture(u_textures[2],  v_texture_uv) * v_color; break;
        case 3 : outputColor = texture(u_textures[3],  v_texture_uv) * v_color; break;
        case 4 : outputColor = texture(u_textures[4],  v_texture_uv) * v_color; break;
        case 5 : outputColor = texture(u_textures[5],  v_texture_uv) * v_color; break;
        case 6 : outputColor = texture(u_textures[6],  v_texture_uv) * v_color; break;
        case 7 : outputColor = texture(u_textures[7],  v_texture_uv) * v_color; break;
        case 8 : outputColor = texture(u_textures[8],  v_texture_uv) * v_color; break;
        case 9 : outputColor = texture(u_textures[9],  v_texture_uv) * v_color; break;
        case 10: outputColor = texture(u_textures[10], v_texture_uv) * v_color; break;
        case 11: outputColor = texture(u_textures[11], v_texture_uv) * v_color; break;
        case 12: outputColor = texture(u_textures[12], v_texture_uv) * v_color; break;
        case 13: outputColor = texture(u_textures[13], v_texture_uv) * v_color; break;
        case 14: outputColor = texture(u_textures[14], v_texture_uv) * v_color; break;
        case 15: outputColor = texture(u_textures[15], v_texture_uv) * v_color; break;
        case 16: outputColor = texture(u_textures[16], v_texture_uv) * v_color; break;
        case 17: outputColor = texture(u_textures[17], v_texture_uv) * v_color; break;
        case 18: outputColor = texture(u_textures[18], v_texture_uv) * v_color; break;
        case 19: outputColor = texture(u_textures[19], v_texture_uv) * v_color; break;
        case 20: outputColor = texture(u_textures[20], v_texture_uv) * v_color; break;
        case 21: outputColor = texture(u_textures[21], v_texture_uv) * v_color; break;
        case 22: outputColor = texture(u_textures[22], v_texture_uv) * v_color; break;
        case 23: outputColor = texture(u_textures[23], v_texture_uv) * v_color; break;
        case 24: outputColor = texture(u_textures[24], v_texture_uv) * v_color; break;
        case 25: outputColor = texture(u_textures[25], v_texture_uv) * v_color; break;
        case 26: outputColor = texture(u_textures[26], v_texture_uv) * v_color; break;
        case 27: outputColor = texture(u_textures[27], v_texture_uv) * v_color; break;
        case 28: outputColor = texture(u_textures[28], v_texture_uv) * v_color; break;
        case 29: outputColor = texture(u_textures[29], v_texture_uv) * v_color; break;
        case 30: outputColor = texture(u_textures[30], v_texture_uv) * v_color; break;
        case 31: outputColor = texture(u_textures[31], v_texture_uv) * v_color; break;
    }
}