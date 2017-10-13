#version 330 core

layout(location = 0) in vec2 position;
layout(location = 2) in vec4 color;
layout(location = 6) in vec2 texture_uv;

layout(location = 0) out vec4 vcolor;
layout(location = 4) out vec2 tex_coord;

uniform mat4 translate;

void main()
{
    vcolor = color;
    tex_coord = texture_uv;
    gl_Position = translate * vec4(position, 0.0, 1.0);
}
