#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex;

uniform mat4 transformation;
uniform mat4 camera;
uniform mat4 perspective;
out vec2 texcoord;

void main(void)
{
texcoord = tex;
gl_Position =  perspective * camera * transformation * vec4(position, 1);
}
