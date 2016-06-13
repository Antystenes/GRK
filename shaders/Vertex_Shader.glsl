#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;

uniform mat4 transformation;
uniform mat4 camera;
uniform mat4 perspective;
uniform vec4 light;
uniform float t;
out vec3 posWorld;
out vec2 texcoord;
out vec3 vnormal;
out vec3 vL;

void main(void)
{
    texcoord = vec2(1.0) - tex;
    vnormal = normal;
    vec3 ppp = position;
    ppp.x += 300*sin(0.001*ppp.z + t);
    posWorld = (camera * transformation * vec4(position, 1)).xyz;
    vec4 ppp2 = camera * transformation * vec4(ppp, 1);
    vL = normalize( (camera * light) - ppp2).rgb;
    gl_Position = perspective * ppp2;
}
