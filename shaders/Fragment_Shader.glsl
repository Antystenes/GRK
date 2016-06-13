#version 430 core

out vec3 color;
in vec3 posWorld;
in vec3 vnormal;
in vec2 texcoord;
//in vec2 noise;
uniform vec3 camPos;
uniform sampler2D text;
uniform sampler2D g1;
uniform sampler2D g2;
const vec3 fogColor = vec3(0.1, 0.2, 0.35);
void main(void)
{
    vec3 waterColor = vec3(0.1, 0.2, 0.35);
    float fog = 1.0 - 1.0 / (1.0 + 2.0*length(posWorld));
    color = texture(text, texcoord).rgb;
    color = mix(color, waterColor, fog);
}
