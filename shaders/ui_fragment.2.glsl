#version 120

varying vec4 vcolor;
varying vec2 tex_coord;

uniform sampler2D tex;
uniform vec4 text_bgnd;

void main()
{
    vec4 fcolor;

    if (tex_coord != vec2(-1000.0, -1000.0))
    {
        fcolor = texture2D(tex, tex_coord);
        fcolor = mix(text_bgnd, vcolor, fcolor);
    }
    else
        fcolor = vcolor;

    gl_FragColor = fcolor;
}
