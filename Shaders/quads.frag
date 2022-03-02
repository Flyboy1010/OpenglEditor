#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_textures[32];

in float v_texture_id;
in vec2 v_texture_uv;
in vec4 v_color;
in vec2 v_screen_pos;

void main() {
    int index = int(v_texture_id);

    switch (index)
    {
        case 0 : color = texture(u_textures[0],  v_texture_uv) * v_color; break;
        case 1 : color = texture(u_textures[1],  v_texture_uv) * v_color; break;
        case 2 : color = texture(u_textures[2],  v_texture_uv) * v_color; break;
        case 3 : color = texture(u_textures[3],  v_texture_uv) * v_color; break;
        case 4 : color = texture(u_textures[4],  v_texture_uv) * v_color; break;
        case 5 : color = texture(u_textures[5],  v_texture_uv) * v_color; break;
        case 6 : color = texture(u_textures[6],  v_texture_uv) * v_color; break;
        case 7 : color = texture(u_textures[7],  v_texture_uv) * v_color; break;
        case 8 : color = texture(u_textures[8],  v_texture_uv) * v_color; break;
        case 9 : color = texture(u_textures[9],  v_texture_uv) * v_color; break;
        case 10: color = texture(u_textures[10], v_texture_uv) * v_color; break;
        case 11: color = texture(u_textures[11], v_texture_uv) * v_color; break;
        case 12: color = texture(u_textures[12], v_texture_uv) * v_color; break;
        case 13: color = texture(u_textures[13], v_texture_uv) * v_color; break;
        case 14: color = texture(u_textures[14], v_texture_uv) * v_color; break;
        case 15: color = texture(u_textures[15], v_texture_uv) * v_color; break;
        case 16: color = texture(u_textures[16], v_texture_uv) * v_color; break;
        case 17: color = texture(u_textures[17], v_texture_uv) * v_color; break;
        case 18: color = texture(u_textures[18], v_texture_uv) * v_color; break;
        case 19: color = texture(u_textures[19], v_texture_uv) * v_color; break;
        case 20: color = texture(u_textures[20], v_texture_uv) * v_color; break;
        case 21: color = texture(u_textures[21], v_texture_uv) * v_color; break;
        case 22: color = texture(u_textures[22], v_texture_uv) * v_color; break;
        case 23: color = texture(u_textures[23], v_texture_uv) * v_color; break;
        case 24: color = texture(u_textures[24], v_texture_uv) * v_color; break;
        case 25: color = texture(u_textures[25], v_texture_uv) * v_color; break;
        case 26: color = texture(u_textures[26], v_texture_uv) * v_color; break;
        case 27: color = texture(u_textures[27], v_texture_uv) * v_color; break;
        case 28: color = texture(u_textures[28], v_texture_uv) * v_color; break;
        case 29: color = texture(u_textures[29], v_texture_uv) * v_color; break;
        case 30: color = texture(u_textures[30], v_texture_uv) * v_color; break;
        case 31: color = texture(u_textures[31], v_texture_uv) * v_color; break;
    }
}