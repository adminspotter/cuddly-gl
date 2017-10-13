#version 330 core

layout(location = 0) in vec4 vcolor;
layout(location = 4) in vec2 tex_coord;

layout(location = 0) out vec4 fcolor;

uniform sampler2D tex;
uniform uint use_text;
uniform vec4 text_bgnd;

void main()
{
    if (tex_coord != vec2(-1000.0, -1000.0))
    {
        fcolor = texture(tex, tex_coord);
        if (use_text == 1u)
            fcolor = mix(text_bgnd, vcolor, fcolor.r);
    }
    else
        fcolor = vcolor;
}
