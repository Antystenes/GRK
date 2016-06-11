#version 430 core

out vec3 color;
in vec2 texcoord;
uniform sampler2D text;
//in vec4 vNormal;
//in vec4 vL;
void main(void)
{
color = texture(text, texcoord).rgb;
// * max (dot(vNormal, vL),0);
}
