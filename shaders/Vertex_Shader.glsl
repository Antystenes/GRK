#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;

uniform mat4 transformation;
uniform mat4 camera;
uniform mat4 perspective;
out vec2 texcoord;
out float f;
out vec3 vL;
out vec3 vnormal;
const vec4 light = vec4(10000.0, 5000.0, 0.0, 1.0);

void main(void)
{
vec4 pos = camera * transformation * vec4(position, 1);
f = exp(-0.6*length(pos));
vL = normalize(pos - (camera * light)).rgb;
texcoord = tex;
vnormal = normal;
gl_Position =  perspective * pos;
}
