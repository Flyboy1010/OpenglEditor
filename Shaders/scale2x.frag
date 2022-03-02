#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_texture;

in vec2 v_texture_uv;

const float offset_x = 1.0 / 320.0;
const float offset_y = 1.0 / 200.0;

void main()
{
    vec4 A = texture(u_texture, v_texture_uv + vec2(0.0, offset_y));
    vec4 B = texture(u_texture, v_texture_uv + vec2(offset_x, 0.0));
    vec4 C = texture(u_texture, v_texture_uv + vec2(-offset_x, 0.0));
    vec4 D = texture(u_texture, v_texture_uv + vec2(0.0, -offset_y));
    vec4 E = texture(u_texture, v_texture_uv + vec2(0.0, 0.0));

    vec2 p = v_texture_uv * vec2(320, 200);
    // p = the position within a pixel in screen space
    p = fract(p);
     if (p.x > 0.5) {
        if (p.y > 0.5) {
            // Top Right

            color = (A==B && A!=C && B!=D) ? B : E;
        } else {
            // Bottom Right

            color = (B==D && B!=A && D!=C) ? D : E;
        }
    } else {
        if (p.y > 0.5) {
            // Top Left
            color = (C==A && C!=D && A!=B) ? A : E;
        } else {
            // Bottom Left
            color = (D==C && D!=B && C!=A) ? C : E;
        }
    }
}