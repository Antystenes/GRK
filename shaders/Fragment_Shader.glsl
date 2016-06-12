
#version 430 core

out vec3 color;
in vec2 texcoord;
in vec2 noise;
in float f;
uniform sampler2D text;
//in vec4 vNormal;
//in vec4 vL;
const vec3 fog = vec3(0.0, 0.0, 0.5);

void main(void)
{
color = f * texture(text, texcoord).rgb + (1-f) * fog;
// * max (dot(vNormal, vL),0);
}
