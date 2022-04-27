#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_texture;

in vec2 v_texture_uv;

const float offset_x = 1.0 / 320.0;
const float offset_y = 1.0 / 200.0;

const float one_third = 1.0 / 3.0;
const float two_thirds = 2.0 / 3.0;

void main()
{
    vec4 A = texture(u_texture, v_texture_uv + vec2(-offset_x, offset_y));
    vec4 B = texture(u_texture, v_texture_uv + vec2(0.0, offset_y));
    vec4 C = texture(u_texture, v_texture_uv + vec2(offset_x, offset_y));
    vec4 D = texture(u_texture, v_texture_uv + vec2(-offset_x, 0.0));
    vec4 E = texture(u_texture, v_texture_uv + vec2(0.0, 0.0));
    vec4 F = texture(u_texture, v_texture_uv + vec2(offset_x, 0.0));
    vec4 G = texture(u_texture, v_texture_uv + vec2(-offset_x, -offset_y));
    vec4 H = texture(u_texture, v_texture_uv + vec2(0.0, -offset_y));
    vec4 I = texture(u_texture, v_texture_uv + vec2(offset_x, -offset_y));

    vec2 p = v_texture_uv * vec2(320, 200);

    // p = the position within a pixel in screen space

    p = fract(p);

    // choose texel

    if (p.x > two_thirds)
    {
        if (p.y > two_thirds)
        {
            // top right

            color = (B==F && B!=D && F!=H) ? F : E;
        }
        else if (p.y > one_third)
        {
            // center right

            color = ((B==F && B!=D && F!=H && E!=I) || (F==H && F!=B && H!=D && E!=C)) ? F : E;
        }
        else
        {
            // bottom right

            color = (F==H && F!=B && H!=D) ? F : E;
        }
    }
    else if (p.x > one_third)
    {
        if (p.y > two_thirds)
        {
            // center top

            color = ((D==B && D!=H && B!=F && E!=C) || (B==F && B!=D && F!=H && E!=A)) ? B : E;
        }
        else if (p.y > one_third)
        {
            // center center

            color = E;
        }
        else
        {
            // center bottom

            color = ((F==H && F!=B && H!=D && E!=G) || (H==D && H!=F && D!=B && E!=I)) ? H : E;
        }
    }
    else 
    {
        if (p.y > two_thirds)
        {
            // left top

            color = (D==B && D!=H && B!=F) ? D : E;
        }
        else if (p.y > one_third)
        {
            // left center

            color = ((H==D && H!=F && D!=B && E!=A) || (D==B && D!=H && B!=F && E!=G)) ? D : E;
        }
        else
        {
            // left bottom

            color = (H==D && H!=F && D!=B) ? D : E;
        }
    }
}