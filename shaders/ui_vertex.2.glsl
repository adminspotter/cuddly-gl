#version 120

attribute vec2 position;
attribute vec4 color;
attribute vec2 texture_uv;

varying vec4 vcolor;
varying vec2 tex_coord;

uniform mat4 translate;

void main()
{
    vcolor = color;
    tex_coord = texture_uv;
    gl_Position = translate * vec4(position, 0.0, 1.0);
}
