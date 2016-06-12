
#version 430 core

out vec3 color;
in vec2 texcoord;
in vec2 noise;
in float f;
uniform sampler2D text;
in vec3 vnormal;
in vec3 vL;
const vec3 fog = vec3(0.0, 0.0, 0.5);

void main(void)
{
color = (f * texture(text, texcoord).rgb + (1-f) * fog) * max (dot(vnormal, vL),0);
}
