#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texture_uv;

out vec4 vcolor;
out vec2 tex_coord;

uniform mat4 translate;

void main()
{
    vcolor = color;
    tex_coord = texture_uv;
    gl_Position = translate * vec4(position, 0.0, 1.0);
}
