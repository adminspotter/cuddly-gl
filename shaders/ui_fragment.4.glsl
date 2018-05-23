#version 330 core

in vec4 vcolor;
in vec2 tex_coord;

layout(location = 0, index = 0) out vec4 fcolor;

uniform sampler2D tex;
uniform vec4 text_bgnd;

void main()
{
    if (tex_coord != vec2(-1000.0, -1000.0))
    {
        fcolor = texture(tex, tex_coord);
        fcolor = mix(text_bgnd, vcolor, fcolor);
    }
    else
        fcolor = vcolor;
}
