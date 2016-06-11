#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex;

uniform mat4 transformation;
uniform mat4 camera;
out vec2 texcoord;

void main(void)
{
texcoord = tex;
gl_Position =  camera * transformation * vec4(position, 1);
}
