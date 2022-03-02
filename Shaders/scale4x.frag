#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_texture;
uniform vec2 u_input_resolution;

in vec2 v_texture_uv;

vec4 scale2x(sampler2D t, vec2 pos, vec2 offset)
{
    vec4 outpu_color;

    vec4 A = texture(t, pos + vec2(0.0, offset.y));
    vec4 B = texture(t, pos + vec2(offset.x, 0.0));
    vec4 C = texture(t, pos + vec2(-offset.x, 0.0));
    vec4 D = texture(t, pos + vec2(0.0, -offset.y));
    vec4 E = texture(t, pos + vec2(0.0, 0.0));

    vec2 p = pos * u_input_resolution;

    // p = the position within a pixel in screen space

    p = fract(p);

     if (p.x > 0.5) {
        if (p.y > 0.5) {
            // Top Right

            outpu_color = (A==B && A!=C && B!=D) ? B : E;
        } else {
            // Bottom Right

            outpu_color = (B==D && B!=A && D!=C) ? D : E;
        }
    } else {
        if (p.y > 0.5) {
            // Top Left

            outpu_color = (C==A && C!=D && A!=B) ? A : E;
        } else {
            // Bottom Left

            outpu_color = (D==C && D!=B && C!=A) ? C : E;
        }
    }

    return outpu_color;
}

void main()
{
    vec2 offset = 1.0 / u_input_resolution;
    vec2 offset2x = 1.0 / (2.0 * u_input_resolution);

    vec4 A = scale2x(u_texture, v_texture_uv + vec2(0.0, offset2x.y), offset);
    vec4 B = scale2x(u_texture, v_texture_uv + vec2(offset2x.x, 0.0), offset);
    vec4 C = scale2x(u_texture, v_texture_uv + vec2(-offset2x.x, 0.0), offset);
    vec4 D = scale2x(u_texture, v_texture_uv + vec2(0.0, -offset2x.y), offset);
    vec4 E = scale2x(u_texture, v_texture_uv + vec2(0.0, 0.0), offset);

    vec2 p = v_texture_uv * u_input_resolution * 2.0;

    // p = the position within a pixel [0...1]

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