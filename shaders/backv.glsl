#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;

uniform mat4 transformation;
uniform mat4 camera;
uniform mat4 perspective;
out vec3 posWorld;
out vec3 texcoord;
out vec3 vnormal;
out vec3 ray;

void main(void)
{
    //texcoord = vec2(1.0) - tex;
    vnormal = normal;
    posWorld = (camera * transformation * vec4(position, 1)).xyz;
    texcoord = position;
    gl_Position = vec4(position, 1);
}
