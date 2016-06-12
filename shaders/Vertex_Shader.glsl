#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex;

uniform mat4 transformation;
uniform mat4 camera;
uniform mat4 perspective;
out vec2 texcoord;
out float f;

void main(void)
{
vec4 pos = camera * transformation * vec4(position, 1);
f = exp(-0.3*length(pos));
texcoord = tex;
gl_Position =  perspective * pos;
}
